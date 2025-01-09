// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "RogueSkeleton.generated.h"

UCLASS()
class UNREALPROJECT_API ARogueSkeleton : public AEnemyUnit {
	GENERATED_BODY()

	// ARogueSkeleton();
	
	protected:
		virtual void BeginPlay() override;
		// void PostInitializeComponents() override;
};