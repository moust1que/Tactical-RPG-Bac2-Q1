// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerUnit.h"
#include "Knight.generated.h"

UCLASS()
class UNREALPROJECT_API AKnight : public APlayerUnit {
	GENERATED_BODY()

	protected:
		virtual void BeginPlay() override;
};