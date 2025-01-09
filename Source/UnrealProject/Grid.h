// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "Grid.generated.h"

UCLASS()
class UNREALPROJECT_API AGrid : public AActor {
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		AGrid();

		UFUNCTION()
		TArray<AGridCell*> GetCellsInRange(AGridCell* CurCell, int32 Range);

		UPROPERTY(EditAnywhere, Category = "Grid") TSubclassOf<AActor> CellActorClass;
		UPROPERTY(EditAnywhere, Category = "Grid") int32 GridSize = 15;

		UPROPERTY() TArray<AGridCell*> GridCells;

		FVector OddRToCubic(int32 X, int32 Y);
		bool IsPathAvailable(AGridCell* startCell, AGridCell* targetCell, int32 Range);
		int32 ManhattanDistanceOddR(int32 x1, int32 y1, int32 x2, int32 y2);
		double euclideanDistance(int32 x1, int32 y1, int32 x2, int32 y2);

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	private:
		UFUNCTION() void GenerateGrid();
		UFUNCTION() void DetectObstacles(UWorld* world);
		UPROPERTY() USceneComponent* GridRoot;
		UPROPERTY() int32 CellSizeX = 170, CellSizeY = 200;
};