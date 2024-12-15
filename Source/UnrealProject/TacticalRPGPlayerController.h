// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "FreeCamera.h"
#include "TacticalRPGPlayerController.generated.h"

UCLASS()
class UNREALPROJECT_API ATacticalRPGPlayerController : public APlayerController {
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, Category = "Input") TSoftObjectPtr<UInputMappingContext> IMC_CameraControl;
		UPROPERTY(EditAnywhere, Category = "Input") TSoftObjectPtr<UInputMappingContext> IMC_UnitControl;

		UPROPERTY(EditAnywhere, Category = "Input") TArray<UInputAction*> InputActions = {};
		UPROPERTY(EditAnywhere, Category = "Input") TMap<FName, FName> ActionFunctionMapping = {};

	protected:
		virtual void BeginPlay() override;
		virtual void SetupInputComponent() override;

	private:
		void DynamicInputHandler(const FInputActionInstance& Instance);
		void SetupInputHandling(UInputComponent* PlayerInputComponent);

		TSoftObjectPtr<UInputMappingContext> CurrentInputMappingContext;
};