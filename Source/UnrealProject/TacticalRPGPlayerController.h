#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "TacticalRPGPlayerController.generated.h"

UCLASS()
class UNREALPROJECT_API ATacticalRPGPlayerController : public APlayerController {
	GENERATED_BODY()

	public:
		// Mapping context de controle de la camera
		UPROPERTY(EditAnywhere, Category = "Input") TSoftObjectPtr<class UInputMappingContext> IMC_CameraControl;

		// Input Actions de la camera
		UPROPERTY(EditAnywhere, Category = "Input") TArray<class UInputAction*> InputActions = {};

		// Nom des actions et des fonctions correspondantes
		UPROPERTY(EditAnywhere, Category = "Input") TMap<FName, FName> ActionFunctionMapping = {};

	protected:
		virtual void BeginPlay() override;
		virtual void SetupInputComponent() override;

	private:
		// Fonction de gestion dynamique des inputs actions
		void DynamicInputHandler(const FInputActionInstance& Instance);

		// Fonction de setup des inputs actions
		void SetupInputHandling(class UInputComponent* PlayerInputComponent);
};