// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TacticalRPGPlayerController.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "Grid.h"
#include "GridCell.h"
#include "Blueprint/UserWidget.h"
#include "BaseCharacter.h"
#include "TacticalRPGGameMode.generated.h"

UENUM()
enum class EGameState : uint8 {
	Starting,
	PlacingUnits,
	PlayPhase,
	EndPhase
};

UENUM()
enum class EUnitType : uint8 {
    Knight,
    Rogue,
    Mage,
    Barbarian
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
		void SelectUnitToSpawn(EUnitType UnitType);
		UFUNCTION()
		void HandleCellClick(AGridCell* ClickedCell);

		UPROPERTY(EditAnywhere, Category = "Grid") TSubclassOf<AGrid> GridRef;

		UPROPERTY(EditAnywhere, Category = "Widget") TSubclassOf<UUserWidget> PlacingMenuWidgetClass;
		UPROPERTY(EditAnywhere, Category = "Widget") TSubclassOf<UUserWidget> BattleUIWidgetClass;

		UPROPERTY(EditAnywhere, Category = "Heros") TSubclassOf<APlayerUnit> Knight;
		UPROPERTY(EditAnywhere, Category = "Heros") TSubclassOf<APlayerUnit> Rogue;
		UPROPERTY(EditAnywhere, Category = "Heros") TSubclassOf<APlayerUnit> Mage;
		UPROPERTY(EditAnywhere, Category = "Heros") TSubclassOf<APlayerUnit> Barbarian;

		UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<AEnemyUnit> RogueSkeleton;
		UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<AEnemyUnit> MageSkeleton;
		UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<AEnemyUnit> MinionSkeleton;
		UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<AEnemyUnit> WarriorSkeleton;

		UPROPERTY(BlueprintReadWrite) TArray<ABaseCharacter*> AllUnits;

		UPROPERTY() bool bPlacingUnits = false;

		UFUNCTION() void RegisterUnit(ABaseCharacter* Unit);

		UPROPERTY() AGrid* Grid;

	private:
		void HandleGameStateChange(EGameState NewGameState);

		UPROPERTY() APlayerController* PlayerController;

		UPROPERTY() int32 UnitsPlaced = 0;
		UPROPERTY() int32 MaxUnits = 4;

		UPROPERTY() TArray<AGridCell*> GridCells;

		UPROPERTY() UUserWidget* PlacingMenuWidget;
		UPROPERTY() UUserWidget* BattleUIWidget;
		UPROPERTY() TSubclassOf<APlayerUnit> CurrentUnitType;

		void ShowPlacingMenu();
		void HidePlacingMenu();

		void ShowBattleUI();
		void HideBattleUI();

		void SortUnitsBySpeed();
		void StartTurnSystem();
		void StartTurnForUnit(ABaseCharacter* Unit);

		void PlaceUnit(TSubclassOf<ABaseCharacter> UnitTypeToSpawn, bool bIsHero, AGridCell* CellToSpawnOn);

		UPROPERTY() bool bCombatActive = true;

		void EndGame();
};