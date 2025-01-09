// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridCell.h"
#include "BaseCharacter.generated.h"

UCLASS()
class UNREALPROJECT_API ABaseCharacter : public APawn {
	GENERATED_BODY()

	public:
		// Sets default values for this pawn's properties
		ABaseCharacter();

		// // Called every frame
		// virtual void Tick(float DeltaTime) override;

		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		UFUNCTION()
		virtual void TakeTurn(AGrid* Grid);
		
		bool bIsHero;
		AGridCell* CurCell;

		int32 TurnSpeed;
		int32 TurnProgress;
		int32 DisplacementRange;
		int32 AttackRange;
		int32 Health;
		int32 Damage;
		int32 Critical;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI") UTexture2D* PortraitTexture;

	// protected:
	// 	// Called when the game starts or when spawned
	// 	virtual void BeginPlay() override;
};