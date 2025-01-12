// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Grid.h"
#include "TacticalRPGGameMode.h"

// Sets default values
ABaseCharacter::ABaseCharacter() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();

	if(USkeletalMeshComponent* mesh = FindComponentByClass<USkeletalMeshComponent>()) {
		if(UBaseCharacterAnimInstance* animInstance = Cast<UBaseCharacterAnimInstance>(mesh->GetAnimInstance())) {
			AnimInstance = animInstance;
		}
	}

	Grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::TakeTurn() {}

void ABaseCharacter::ReveiveDamage(int32 DamageReceived) {
	Health -= DamageReceived + FMath::FloorToInt((LastAttacker->CurLevel - 1) * 0.2f * DamageReceived);

	AnimInstance->IsHit = true;

	FTimerHandle HitTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(HitTimerHandle, [this]() {
		AnimInstance->IsHit = false;
	}, 1.0f, false);

	// Déclenche l'événement OnHealthChanged
    OnHealthChanged.Broadcast(Health, MaxHealth);

	if(Health <= 0) {
		OnDeath();
	}
}

void ABaseCharacter::MoveAlongPath(TArray<AGridCell*> path) {
	UE_LOG(LogTemp, Warning, TEXT("MoveAlongPath"));

	if(remainingDisplacement <= 0) {
		return;
	}

	AnimInstance->IsRunning = true;

    Grid->ResetHighlightedCells();

    CurDisplacementUsed += path.Num() - 1;

	Path = path;
	CurrentCellIndex = 0;
	ElapsedTime = 0.0f;

	StartPosition = CurCell->GetActorLocation();
	TargetPosition = Path[CurrentCellIndex + 1]->GetActorLocation();
}

void ABaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(Path.Num() > 0 && CurrentCellIndex < Path.Num() - 1) {
		ElapsedTime += DeltaTime;

		float distanceToTravel = FVector::Dist(StartPosition, TargetPosition);
		float timeToTravel = distanceToTravel / MoveSpeed;

		FRotator targetRotation = FRotator::ZeroRotator;

		if(CurrentCellIndex < Path.Num() - 1) {
			FVector direction = TargetPosition - GetActorLocation();
			direction.Z = 0.0f;
			targetRotation = direction.Rotation() - FRotator(0.0f, 90.0f, 0.0f);;
		}

		FRotator currentRotation = GetActorRotation();
		FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 10.0f);
		SetActorRotation(newRotation);

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
			float alpha = ElapsedTime / timeToTravel;
			FVector CurrentPosition = FMath::Lerp(StartPosition, TargetPosition, alpha);
			SetActorLocation(CurrentPosition);
		}

		if(CurCell == Path[Path.Num() - 1] && AnimInstance->IsRunning) {
			AnimInstance->IsRunning = false;

			if(!bIsHero && !bWillAttack) {
				ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				gameMode->BattleUIWidget->EndTurn();
				bWillAttack = true;
			}else if(bIsHero && CurDisplacementUsed > 0) {
				Grid->HighlightCellsInRange(CurCell, remainingDisplacement);
			}
		}
	}else if(CurrentCellIndex >= Path.Num() - 1 && TargetToAttack != nullptr) {
		Attack(TargetToAttack);
		TargetToAttack = nullptr;
	}
}

void ABaseCharacter::MoveToCell(AGridCell* NextCell) {
	NextCell->SetState(bIsHero ? ECellState::OccupiedHero : ECellState::OccupiedEnemy);
	NextCell->SetOccupant(this);
	CurCell->SetState(ECellState::Empty);
	CurCell->SetOccupant(nullptr);
	CurCell = NextCell;
}

void ABaseCharacter::Attack(ABaseCharacter* Target) {
    if(Target && bCanAttack) {
        bCanAttack = false;

		AnimInstance->IsAttacking = true;
        
		FVector direction = Target->GetActorLocation() - GetActorLocation();
		direction.Z = 0.0f;

		FRotator targetRotation = direction.Rotation() - FRotator(0.0f, 90.0f, 0.0f);
		SetActorRotation(targetRotation);

		Target->LastAttacker = this;

        Target->ReveiveDamage(Damage);

		FTimerHandle AttackTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [this]() {
			AnimInstance->IsAttacking = false;

			if(!bIsHero) {
				ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				gameMode->BattleUIWidget->EndTurn();
			}
		}, 1.0f, false);
    }
}

void ABaseCharacter::ResetRange() {
	Grid->ResetHighlightedCells();
    if(remainingDisplacement > 0) {
        Grid->HighlightCellsInRange(CurCell, remainingDisplacement);
    }
}

void ABaseCharacter::OnDeath() {
	AnimInstance->IsDead = true;

	UWidgetComponent* HealthBarWidget = Cast<UWidgetComponent>(GetDefaultSubobjectByName(TEXT("HealthBar")));
	HealthBarWidget->SetVisibility(false);

	CurCell->SetState(ECellState::Empty);

	ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if(gameMode) {
		gameMode->RemoveUnit(this);
	}

	LastAttacker->GiveXP();
	LastAttacker->ResetRange();

	FTimerHandle DeathTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]() {
		Destroy();
	}, 3.0f, false);
}

void ABaseCharacter::GiveXP() {
    XP += 60;

    while(XP >= XPForNextLevel) {
        XP -= XPForNextLevel;
        XPForNextLevel = FMath::FloorToInt(XPForNextLevel * 1.2f);
        CurLevel++;
    }

    OnXPChanged.Broadcast(XP, XPForNextLevel);
	UE_LOG(LogTemp, Warning, TEXT("Level up"));
}