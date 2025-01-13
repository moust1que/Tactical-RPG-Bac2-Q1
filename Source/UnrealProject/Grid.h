#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "Grid.generated.h"

UCLASS()
class UNREALPROJECT_API AGrid : public AActor {
	GENERATED_BODY()
	
	public:	
		AGrid();

		// Fonction retournant les cellules dans une portée
		UFUNCTION() TArray<AGridCell*> GetCellsInRange(AGridCell* CurCell, int32 Range);

		// Classe des cellules de la grille
		UPROPERTY(EditAnywhere, Category = "Grid") TSubclassOf<AActor> CellActorClass;
		// Taille de la grille
		UPROPERTY(EditAnywhere, Category = "Grid") int32 GridSize = 15;

		// Tableau des cellules composant la grille
		UPROPERTY() TArray<AGridCell*> GridCells;

		// Fonction de conversion de coordonnees hexagonales OddR vers Cubic
		FVector OddRToCubic(int32 X, int32 Y);

		// Fonction de verification si un chemin est possible entre deux cellules pour une portee donnee
		bool IsPathAvailable(AGridCell* startCell, AGridCell* targetCell, int32 Range);

		// Distance de Manhattan en coordonnees cubiques
		int32 ManhattanDistanceOddR(int32 x1, int32 y1, int32 x2, int32 y2);

		// Fonction de pathfinding
		TArray<AGridCell*> FindPath(AGridCell* StartCell, AGridCell* TargetCell);

		// Fonction de mise en evidence des cellules dans une portée
		void HighlightCellsInRange(AGridCell* CurCell, int32 RemainingDisplacement);

		// Reinitialisation de la surbrillance des cellules
		UFUNCTION(BlueprintCallable) void ResetHighlightedCells();

		// Fonction retournant les voisins d'une cellule
		UFUNCTION() TArray<AGridCell*> GetNeighbors(AGridCell* Cell);

	protected:
		virtual void BeginPlay() override;

	private:
		// Fonction de generation de la grille
		UFUNCTION() void GenerateGrid();

		// Fonction de detection des obstacles
		UFUNCTION() void DetectObstacles(UWorld* world);

		// Racine de la grille
		UPROPERTY() USceneComponent* GridRoot;

		// Offset des cellules en X et Y
		UPROPERTY() int32 CellOffsetX = 170, CellOffsetY = 200;
};