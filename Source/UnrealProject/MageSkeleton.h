// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "MageSkeleton.generated.h"

UCLASS()
class UNREALPROJECT_API AMageSkeleton : public AEnemyUnit {
	GENERATED_BODY()
	
	protected:
		virtual void BeginPlay() override;
};