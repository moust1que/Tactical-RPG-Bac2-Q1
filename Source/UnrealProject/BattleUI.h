#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUI.generated.h"

// Event delegate de fin de tour
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndTurn);

UCLASS()
class UNREALPROJECT_API UBattleUI : public UUserWidget {
	GENERATED_BODY()

	protected:
		// On recupere le widget turn order
		UPROPERTY(meta = (BindWidget)) class UTurnOrderWidget* WBP_TurnOrder;

	public:
		// Fonction de mise a jour du widget affichants les tours
		UFUNCTION(BlueprintCallable) void CallUpdateTurnUI();

		// Delegue de fin de tour
		UPROPERTY(BlueprintAssignable, Category = "BattleUI") FEndTurn OnEndTurn;

		// Fonction de fin de tour
		void EndTurn();
};