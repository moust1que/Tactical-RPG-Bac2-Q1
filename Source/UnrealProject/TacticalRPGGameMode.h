// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TacticalRPGPlayerController.h"
#include "PlayerUnit.h"
#include "Grid.h"
#include "GridCell.h"
#include "Blueprint/UserWidget.h"
#include "TacticalRPGGameMode.generated.h"

UENUM()
enum class EGameState : uint8 {
	Starting,
	PlacingUnits,
	PlayerPhase,
	EnemyPhase
};

UENUM()
enum class EUnitType : uint8 {
    Knight,
    Rogue,
    Mage,
    Warrior
};

UCLASS()
class UNREALPROJECT_API ATacticalRPGGameMode : public AGameMode {
	GENERATED_BODY()

	protected:
		void BeginPlay() override;
		EGameState CurrentGameState;

	public:
		ATacticalRPGGameMode();

		UFUNCTION(BlueprintCallable)
		void SetGameState(EGameState NewGameState);

		UFUNCTION(BlueprintCallable)
		void PlaceUnit(EUnitType UnitType);
		UFUNCTION()
		void HandleCellClick(AGridCell* ClickedCell);

		UPROPERTY(EditAnywhere, Category = "Grid") TSubclassOf<AGrid> GridRef;

		UPROPERTY(EditAnywhere, Category = "Widget") TSubclassOf<UUserWidget> PlacingMenuWidgetClass;

		UPROPERTY(EditAnywhere, Category = "Units") TSubclassOf<APlayerUnit> Knight;
		UPROPERTY(EditAnywhere, Category = "Units") TSubclassOf<APlayerUnit> Rogue;
		UPROPERTY(EditAnywhere, Category = "Units") TSubclassOf<APlayerUnit> Mage;
		UPROPERTY(EditAnywhere, Category = "Units") TSubclassOf<APlayerUnit> Warrior;

	private:
		void HandleGameStateChange(EGameState NewGameState);

		UPROPERTY() APlayerController* PlayerController;

		UPROPERTY() int32 UnitsPlaced = 0;
		UPROPERTY() int32 MaxUnits = 4;

		UPROPERTY() AGrid* Grid;
		UPROPERTY() TArray<AGridCell*> GridCells;

		UPROPERTY() UUserWidget* PlacingMenuWidget;
		UPROPERTY() TSubclassOf<APlayerUnit> CurrentUnitType;
		UPROPERTY() bool bPlacingUnits = false;
		void ShowPlacingMenu();
		void HidePlacingMenu();
};