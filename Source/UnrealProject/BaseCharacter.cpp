// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "TacticalRPGGameMode.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

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
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::TakeTurn(AGrid* GridRef) {}

void ABaseCharacter::ReveiveDamage(int32 DamageReceived) {
	Health -= DamageReceived;

	// Déclenche l'événement OnHealthChanged
    OnHealthChanged.Broadcast(Health, MaxHealth);

	if(Health <= 0) {
		Die();
	}
}

void ABaseCharacter::Die() {
	CurCell->SetState(ECellState::Empty);
	Destroy();

	ATacticalRPGGameMode* GameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode) {
		GameMode->RemoveUnit(this);
	}
}

void ABaseCharacter::MoveAlongPath(TArray<AGridCell*> path) {
	AnimInstance->IsRunning = true;

    Grid->ResetHighlightedCells();

	bIsMoving = true;

    CurDisplacementUsed += path.Num() - 1;

	Path = path;
	CurrentCellIndex = 0;
	ElapsedTime = 0.0f;

	StartPosition = CurCell->GetActorLocation();
	TargetPosition = path[CurrentCellIndex + 1]->GetActorLocation();
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
	}else if(AnimInstance->IsRunning) {
		AnimInstance->IsRunning = false;

		if(remainingDisplacement > 0) {
			Grid->HighlightCellsInRange(CurCell, remainingDisplacement);
		}
	}

	if(CurrentCellIndex >= Path.Num() - 1 && TargetToAttack != nullptr) {
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
        
		FVector direction = Target->GetActorLocation() - GetActorLocation();
		direction.Z = 0.0f;

		FRotator targetRotation = direction.Rotation() - FRotator(0.0f, 90.0f, 0.0f);
		SetActorRotation(targetRotation);

        Target->ReveiveDamage(Damage);
    }
}