#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TacticalRPGGameMode.generated.h"

// Etats du jeu
UENUM()
enum class EGameState : uint8 {
	Starting,
	PlacingUnits,
	PlayPhase
};

// Types de personnages
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

	public:
		// Constructeur
		ATacticalRPGGameMode();

		// Defini l'etat du jeu
		UFUNCTION(BlueprintCallable)
		void SetGameState(EGameState NewGameState);

		// Defini le type de personnage a placer
		UFUNCTION(BlueprintCallable)
		void SelectUnitToSpawn(EUnitType UnitType);

		// Gere le clic sur une cellule
		UFUNCTION()
		void HandleCellClick(AGridCell* ClickedCell);

		// Reference de la classe de la grille
		UPROPERTY(EditAnywhere, Category = "Grid") TSubclassOf<class AGrid> GridRef;

		// Reference des classes des widgets
		UPROPERTY(EditAnywhere, Category = "Widget") TSubclassOf<UUserWidget> PlacingMenuWidgetClass;
		UPROPERTY(EditAnywhere, Category = "Widget") TSubclassOf<UUserWidget> BattleUIWidgetClass;
		UPROPERTY(EditAnywhere, Category = "Widget") TSubclassOf<UUserWidget> VictoryWidgetClass;
		UPROPERTY(EditAnywhere, Category = "Widget") TSubclassOf<UUserWidget> DefeatWidgetClass;

		// Reference des classes des personnages
		UPROPERTY(EditAnywhere, Category = "Heros") TSubclassOf<class APlayerUnit> Knight;
		UPROPERTY(EditAnywhere, Category = "Heros") TSubclassOf<class APlayerUnit> Rogue;
		UPROPERTY(EditAnywhere, Category = "Heros") TSubclassOf<class APlayerUnit> Mage;
		UPROPERTY(EditAnywhere, Category = "Heros") TSubclassOf<class APlayerUnit> Barbarian;
		UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<class AEnemyUnit> RogueSkeleton;
		UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<class AEnemyUnit> MageSkeleton;
		UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<class AEnemyUnit> MinionSkeleton;
		UPROPERTY(EditAnywhere, Category = "Enemies") TSubclassOf<class AEnemyUnit> WarriorSkeleton;

		// Tableau contenant tous les personnages en jeu
		UPROPERTY(BlueprintReadWrite) TArray<class ABaseCharacter*> AllUnits;

		// Booleen indiquant si le placement des personnages est en cours
		UPROPERTY() bool bPlacingUnits = false;

		// Fonction permettant d'enregistrer un nouveau personnage
		UFUNCTION() void RegisterUnit(class ABaseCharacter* Unit);

		// Fonction permettant de supprimer un personnage
		UFUNCTION() void RemoveUnit(class ABaseCharacter* Unit);

		// Reference de la grille
		UPROPERTY(BlueprintReadOnly) AGrid* Grid;

		// Fonction permettant de trier les personnages par vitesse
		UFUNCTION(BlueprintCallable) void SortUnitsBySpeed();

		// Fonction permettant de commencer le tour d'un personnage
		UFUNCTION(BlueprintCallable) void StartTurnForUnit(class ABaseCharacter* Unit);

		// Reference des widgets
		UPROPERTY() UUserWidget* PlacingMenuWidget;
		UPROPERTY() class UBattleUI* BattleUIWidget;
		UPROPERTY() UUserWidget* VictoryWidget;
		UPROPERTY() UUserWidget* DefeatWidget;

		// Fonctions de fin de partie
		void Defeat();
		void Victory();

	private:
		// Fonction permettant de gerer le changement d'etat du jeu
		void HandleGameStateChange(EGameState NewGameState);

		// Reference du player controller
		UPROPERTY() APlayerController* PlayerController;

		// Nombre d'unites placees
		UPROPERTY() int32 UnitsPlaced = 0;

		// Nombre d'unites maximales que le joueur peut placer
		UPROPERTY() int32 MaxUnits = 4;

		// Tableau des cellules composant la grille
		UPROPERTY() TArray<AGridCell*> GridCells;

		// Type de personnage en cours de placement
		UPROPERTY() TSubclassOf<APlayerUnit> CurrentUnitType;

		// Fonction D'affichage du menu de placement de personnages
		void ShowPlacingMenu();
		// Fonction permettantr de cacher le menu de placement de personnages
		void HidePlacingMenu();

		// Fonction permettant d'afficher l'UI de combat
		void ShowBattleUI();

		// Fonction permettant de commencer le systeme de tour
		void StartTurnSystem();

		// Fonction permettant de placer un personnage
		void PlaceUnit(TSubclassOf<class ABaseCharacter> UnitTypeToSpawn, bool bIsHero, AGridCell* CellToSpawnOn);

		// Etat actuel du jeu
		EGameState CurrentGameState;
};