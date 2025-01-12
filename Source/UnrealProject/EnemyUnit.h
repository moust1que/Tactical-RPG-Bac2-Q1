// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyUnit.generated.h"

UCLASS()
class UNREALPROJECT_API AEnemyUnit : public ABaseCharacter {
	GENERATED_BODY()

	// protected:
	// 	virtual void BeginPlay() override;

	public:
		AEnemyUnit();

		void TakeTurn() override;

		class APlayerUnit* FindClosestPlayer(TArray<ABaseCharacter*> AllUnits);
};