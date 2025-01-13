#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "FreeCamera.generated.h"

UCLASS()
class UNREALPROJECT_API AFreeCamera : public APawn {
	GENERATED_BODY()

	public:
		// Constructeur
		AFreeCamera();

		// Fonction permettant l'appelle d'une fonction donnee en parametre
		void HandleFunctionCall(FName FunctionName, const FInputActionInstance& Instance, FName actionName);

		// Variables de la camera
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraSpeed = 100.0f;
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraRotationSpeed = 100.0f;
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraZoomSpeed = 1000.0f;
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraZoomMin = 100.0f;
		UPROPERTY(EditAnywhere, Category = "Camera") float CameraZoomMax = 2000.0f;

	protected:
		virtual void BeginPlay() override;

	private:
		// Composants de la camera
		class USpringArmComponent* SpringArmComp;
		class UCameraComponent* CameraComp;

		// Coordonees des sommets de l'hexagone que forme la map
		TArray<FVector2D> HexVertices = {
			FVector2D(170, 2900),
			FVector2D(-3740, 600),
			FVector2D(-3740, -4000),
			FVector2D(170, -6300),
			FVector2D(4080, -4000),
			FVector2D(4080, 600)
		};

		// Normales des aretes de l'hexagone
		TArray<FVector2D> HexNormals;

		// Fonction de calcul des normales de l'hexagone
		void CalculateHexagonNormals();
		
		// Fonction permettant de savoir si un point est dans l'hexagone
		bool IsPointInHexagon(FVector2D Point);

		// Fonction permettant de fixer le joueur sur l'hexagone s'il essaye de quitter les limites de la map
		FVector2D ClampPawnToHexagon(FVector2D Point);

		// Fonctions de mouvement
		UFUNCTION() void Move(const FInputActionInstance& Instance);
		UFUNCTION() void Rotate(const FInputActionInstance& Instance);
		UFUNCTION() void Zoom(const FInputActionInstance& Instance);
};