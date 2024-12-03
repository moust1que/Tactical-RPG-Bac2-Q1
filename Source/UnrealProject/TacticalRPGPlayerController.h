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
		UPROPERTY(EditAnywhere, Category = "Input") TSoftObjectPtr<UInputMappingContext> InputMappingContext;
		UPROPERTY(EditAnywhere, Category = "Input") UInputAction* IA_CursorMove;

	protected:
		virtual void BeginPlay() override;
		virtual void SetupInputComponent() override;

	private:
		void SetupInputHandling(UInputComponent* PlayerInputComponent);
		void CursorMove(const FInputActionInstance& Instance);
};