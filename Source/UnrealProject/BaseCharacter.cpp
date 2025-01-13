#include "BaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Grid.h"
#include "TacticalRPGGameMode.h"
#include "BaseCharacterAnimInstance.h"
#include "BattleUI.h"

ABaseCharacter::ABaseCharacter() {
	// Permet a ce personnage de "tick" chaque frame pour mettre a jour sa logique
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();

	// Recherche et assigne l'instance d'animation a partir du mesh du personnage
	if(USkeletalMeshComponent* mesh = FindComponentByClass<USkeletalMeshComponent>()) {
		if(UBaseCharacterAnimInstance* animInstance = Cast<UBaseCharacterAnimInstance>(mesh->GetAnimInstance())) {
			AnimInstance = animInstance; // Associe l'instance d'animation au personnage
		}
	}

	// Recupere la grille de jeu en cours a partir du monde
	Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	// Fonction par defaut pour lier les entrees du joueur
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// La fonction est laissee vide. Elle sera override par les enfants de la classe
void ABaseCharacter::TakeTurn() {}

void ABaseCharacter::ReveiveDamage(int32 DamageReceived) {
	// Le personnage reçoit des degâts et modifie sa sante en consequence
	Health -= DamageReceived + FMath::FloorToInt((LastAttacker->CurLevel - 1) * 0.2f * DamageReceived);

	// Declenche une animation d'impact
	AnimInstance->IsHit = true;

	// Temporisation de l'animation pour la jouer jusqu'e la fin
	FTimerHandle HitTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(HitTimerHandle, [this]() {
		AnimInstance->IsHit = false;
	}, 1.0f, false);

	// Declenche l'evenement de changement de sante
    OnHealthChanged.Broadcast(Health, MaxHealth);

	// Si la sante est inferieure ou egale a 0, le personnage meurt
	if(Health <= 0) {
		OnDeath();
	}
}

void ABaseCharacter::MoveAlongPath(TArray<AGridCell*> path) {
	// Si le deplacement restant est nul ou negatif, on ne peut pas se deplacer.
	if(remainingDisplacement <= 0) {
		return;
	}

	// Le personnage commence a se deplacer, l'animation est activee
	AnimInstance->IsRunning = true;

	// Reinitialise les cellules sur la grille
    Grid->ResetHighlightedCells();

	// Calcule combien de cases seront traversees
    CurDisplacementUsed += path.Num() - 1;

	// Initialise les variables qui vont permettrent le deplacement fluide du joueur
	Path = path;
	CurrentCellIndex = 0;
	ElapsedTime = 0.0f;

	// Initialise les positions de depart et de destination pour le deplacement
	StartPosition = CurCell->GetActorLocation();
	TargetPosition = Path[CurrentCellIndex + 1]->GetActorLocation();
}

void ABaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Si le personnage est en train de se deplacer
	if(Path.Num() > 0 && CurrentCellIndex < Path.Num() - 1) {
		// On ajoute le temps passe depuis le dernier tick
		ElapsedTime += DeltaTime;

		// Calcul de la distance a parcourir et du temps necessaire pour le deplacement
		float distanceToTravel = FVector::Dist(StartPosition, TargetPosition);
		float timeToTravel = distanceToTravel / MoveSpeed;

		// Rotation du personnage pour se diriger vers la prochaine case
		FRotator targetRotation = FRotator::ZeroRotator;
		if(CurrentCellIndex < Path.Num() - 1) {
			FVector direction = TargetPosition - GetActorLocation();
			direction.Z = 0.0f;
			targetRotation = direction.Rotation() - FRotator(0.0f, 90.0f, 0.0f);;
		}

		// Rotation progressive du personnage
		FRotator currentRotation = GetActorRotation();
		FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 10.0f);
		SetActorRotation(newRotation);

		// Gestion du passage d'une case a l'autre
		if(ElapsedTime >= timeToTravel) {
			AGridCell* NextCell = Path[CurrentCellIndex + 1];
			MoveToCell(NextCell);

			ElapsedTime = 0.0f;
			CurrentCellIndex++;

			StartPosition = CurCell->GetActorLocation();
			if(CurrentCellIndex < Path.Num() - 1) {
				TargetPosition = Path[CurrentCellIndex + 1]->GetActorLocation();
			}
		}else {
			// Mise a jour de la position du personnage a chaque tick
			float alpha = ElapsedTime / timeToTravel;
			FVector CurrentPosition = FMath::Lerp(StartPosition, TargetPosition, alpha);
			SetActorLocation(CurrentPosition);
		}

		// Une fois arrive a la derniere case, verifie si le personnage doit attaquer ou finir son tour
		if(CurCell == Path[Path.Num() - 1] && AnimInstance->IsRunning) {
			// Fin de l'annimation de deplacement
			AnimInstance->IsRunning = false;

			if(!bIsHero && !bWillAttack) {
				// Si c'est une IA et qu'elle n'attaque pas, on termine le tour
				ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				gameMode->BattleUIWidget->EndTurn();
				bWillAttack = true;
			}else if(bIsHero && CurDisplacementUsed > 0) {
				// Si c'est le joueur et qu'il a encore des deplacements disponibles, on affiche les cases restantes
				Grid->HighlightCellsInRange(CurCell, remainingDisplacement);
			}
		}
	}else if(CurrentCellIndex >= Path.Num() - 1 && TargetToAttack != nullptr) {
		// Si le personnage est arrive a sa destination et a une cible a attaquer, on l'attaque
		Attack(TargetToAttack);
		TargetToAttack = nullptr;
	}
}

void ABaseCharacter::MoveToCell(AGridCell* NextCell) {
	// Gestion des etats des cellules au passage du joueur
	NextCell->SetState(bIsHero ? ECellState::OccupiedHero : ECellState::OccupiedEnemy);
	NextCell->SetOccupant(this);
	CurCell->SetState(ECellState::Empty);
	CurCell->SetOccupant(nullptr);
	CurCell = NextCell;
}

void ABaseCharacter::Attack(ABaseCharacter* Target) {
    if(Target && bCanAttack) {
        bCanAttack = false;

		// On lance l'animation d'attaque
		AnimInstance->IsAttacking = true;
        
		// Calcul de la direction vers la cible pour ajuster la rotation
		FVector direction = Target->GetActorLocation() - GetActorLocation();
		direction.Z = 0.0f;

		FRotator targetRotation = direction.Rotation() - FRotator(0.0f, 90.0f, 0.0f);
		SetActorRotation(targetRotation);

		// Le personnage attaque se souvient de l'attaquant
		Target->LastAttacker = this;

		// Inflige des degâts a la cible
        Target->ReveiveDamage(Damage);

		// Temporisation de l'animation d'attaque
		FTimerHandle AttackTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [this]() {
			AnimInstance->IsAttacking = false;

			if(!bIsHero) {
				// Si c'est une IA, on termine son tour quand l'animation se termine
				ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				gameMode->BattleUIWidget->EndTurn();
			}
		}, 1.0f, false);
    }
}

void ABaseCharacter::ResetRange() {
	// Reinitialisation de la surbrillance des cellules
	Grid->ResetHighlightedCells();
    if(remainingDisplacement > 0) {
		// Met en surbrillance les cellules accessibles s'il reste des deplacements
        Grid->HighlightCellsInRange(CurCell, remainingDisplacement);
    }
}

void ABaseCharacter::OnDeath() {
	// Lorsque le personnage meurt, on joue l'animation de mort
	AnimInstance->IsDead = true;

	// Lorsque le personnage meurt, on joue l'animation et on cache la barre de sante
	UWidgetComponent* HealthBarWidget = Cast<UWidgetComponent>(GetDefaultSubobjectByName(TEXT("HealthBar")));
	HealthBarWidget->SetVisibility(false);

	// On libere la cellule qu'il occupait
	CurCell->SetState(ECellState::Empty);

	ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if(gameMode) {
		// On supprime le personnage de la liste des personnages en jeu
		gameMode->RemoveUnit(this);
	}

	// On donne de l'XP au personnage qui a tue
	LastAttacker->GiveXP();
	// On reset la range de deplacement du personnage qui a tue
	LastAttacker->ResetRange();

	FTimerHandle DeathTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]() {
		// On supprime le personnage apres 3 secondes pour profiter un minimum de l'animation de mort du personnage
		Destroy();
	}, 3.0f, false);

	// On verifie si il reste des ennemies en jeu
	CheckIfEnemyStillAlive();
}

void ABaseCharacter::CheckIfEnemyStillAlive() {
	int32 aliveEnemiesUnits = 0;

	ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	for(ABaseCharacter* unit : gameMode->AllUnits) {
		if(!unit->bIsHero) {
			aliveEnemiesUnits++;
		}
	}

	if(aliveEnemiesUnits == 0) {
		// S'il ne reste pas d'ennemis en jeu, on declenche la victoire
		gameMode->Victory();
	}
}

void ABaseCharacter::GiveXP() {
	// On donne 60 XP au personnage qui a tue
    XP += 60;

    while(XP >= XPForNextLevel) {
		// On augmente le niveau du personnage et on augmente la quantite d'XP pour le niveau suivant
        XP -= XPForNextLevel;
        XPForNextLevel = FMath::FloorToInt(XPForNextLevel * 1.2f);
        CurLevel++;
    }

	// On appel la fonction de mise a jour de la barre d'xp
    OnXPChanged.Broadcast(XP, XPForNextLevel);
}