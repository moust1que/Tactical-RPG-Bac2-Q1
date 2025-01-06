// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerUnit.generated.h"

UCLASS()
class UNREALPROJECT_API APlayerUnit : public ABaseCharacter {
	GENERATED_BODY()

	protected:
		virtual void BeginPlay() override;
};