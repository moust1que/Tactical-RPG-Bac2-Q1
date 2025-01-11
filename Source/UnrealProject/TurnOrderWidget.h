// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "TurnOrderWidget.generated.h"

UCLASS()
class UNREALPROJECT_API UTurnOrderWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable) void UpdateTurnUI();
		UPROPERTY(BlueprintReadWrite) class ATacticalRPGGameMode* GameMode;
		UPROPERTY(BlueprintReadWrite) TArray<UImage*> TurnImages;
};