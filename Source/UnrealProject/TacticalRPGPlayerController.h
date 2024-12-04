// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "TacticalRPGPlayerController.generated.h"

UCLASS()
class UNREALPROJECT_API ATacticalRPGPlayerController : public APlayerController {
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, Category = "Input") TSoftObjectPtr<UInputMappingContext> IMC_CameraControl;
		UPROPERTY(EditAnywhere, Category = "Input") TSoftObjectPtr<UInputMappingContext> IMC_UnitControl;
		UPROPERTY(EditAnywhere, Category = "Input") UInputAction* IA_CameraMove;

	protected:
		virtual void BeginPlay() override;
		virtual void SetupInputComponent() override;

	private:
		void SetupInputHandling(UInputComponent* PlayerInputComponent);
		void CameraMove(const FInputActionInstance& Instance);
};