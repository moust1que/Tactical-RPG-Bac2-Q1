// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "FreeCamera.generated.h"

UCLASS()
class UNREALPROJECT_API AFreeCamera : public APawn {
	GENERATED_BODY()

	public:
		// Sets default values for this pawn's properties
		AFreeCamera();

		// Called every frame
		virtual void Tick(float DeltaTime) override;

		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		void HandleFunctionCall(FName FunctionName, const FInputActionInstance& Instance, FName actionName);

		UPROPERTY(EditAnywhere, Category = "Camera") float CameraSpeed = 100.0f;
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraRotationSpeed = 100.0f;
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraZoomSpeed = 1000.0f;
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraZoomMin = 100.0f;
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraZoomMax = 2000.0f;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	private:
		USpringArmComponent* SpringArmComp;
		UCameraComponent* CameraComp;

		TArray<FVector2D> HexVertices = {
			FVector2D(170, 2900),
			FVector2D(-3740, 600),
			FVector2D(-3740, -4000),
			FVector2D(170, -6300),
			FVector2D(4080, -4000),
			FVector2D(4080, 600)
		};

		TArray<FVector2D> HexNormals;

		void CalculateHexagonNormals();
		bool IsPointInHexagon(FVector2D Point);
		FVector2D ClampPawnToHexagon(FVector2D Point);

		UFUNCTION() void Move(const FInputActionInstance& Instance);
		UFUNCTION() void Rotate(const FInputActionInstance& Instance);
		UFUNCTION() void Zoom(const FInputActionInstance& Instance);
};