#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnOrderWidget.generated.h"

UCLASS()
class UNREALPROJECT_API UTurnOrderWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		// Fonction de mise a jour du widget des tours
		UFUNCTION(BlueprintCallable) void UpdateTurnUI();

		// Reference du GameMode
		UPROPERTY(BlueprintReadWrite) class ATacticalRPGGameMode* GameMode;

		// Reference des images
		UPROPERTY(BlueprintReadWrite) TArray<class UImage*> TurnImages;
};