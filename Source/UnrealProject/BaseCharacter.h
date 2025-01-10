// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridCell.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Health, float, MaxHealth);

UCLASS()
class UNREALPROJECT_API ABaseCharacter : public APawn {
	GENERATED_BODY()

	public:
		// Sets default values for this pawn's properties
		ABaseCharacter();

		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		UFUNCTION()
		virtual void TakeTurn(AGrid* GridRef);
		
		bool bIsHero;
		AGridCell* CurCell;

		AGrid* Grid;

		int32 TurnSpeed;
		int32 TurnProgress;
		int32 DisplacementRange;
		int32 AttackRange;
		UPROPERTY(BlueprintReadOnly)int32 Health;
		UPROPERTY(BlueprintReadOnly)int32 MaxHealth;
		int32 Damage;

		int32 CurDisplacementUsed = 0;
		int32 remainingDisplacement = 0;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI") UTexture2D* PortraitTexture;

		void ReveiveDamage(int32 DamageReceived);

		void MoveAlongPath(TArray<AGridCell*> path);

		// Delegate pour notifier des changements de santé
		UPROPERTY(BlueprintAssignable, Category = "Health")
		FOnHealthChanged OnHealthChanged;

		void Die();

		virtual void Tick(float DeltaTime) override;

		TArray<AGridCell*> Path;

		bool bIsMoving = false;

		ABaseCharacter* TargetToAttack = nullptr;

		void Attack(ABaseCharacter* Enemy);

		bool bCanAttack = true;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsRunning = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsAttacking = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsHit = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsDead = false;

	private:
		float ElapsedTime = 0.0f;
		int32 CurrentCellIndex = 0;

		void MoveToCell(AGridCell* NextCell);

		float MoveSpeed = 200.0f;
		FVector StartPosition;
		FVector TargetPosition;
};