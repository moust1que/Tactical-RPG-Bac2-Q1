// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndTurn);

UCLASS()
class UNREALPROJECT_API UBattleUI : public UUserWidget {
	GENERATED_BODY()

	protected:
		UPROPERTY(meta = (BindWidget)) class UTurnOrderWidget* WBP_TurnOrder;

	public:
		UFUNCTION(BlueprintCallable) void CallUpdateTurnUI();

		UPROPERTY(BlueprintAssignable, Category = "BattleUI") FEndTurn OnEndTurn;

		void EndTurn();
};