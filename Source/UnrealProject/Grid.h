// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "Grid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGridCpp, Log, All);

UCLASS()
class UNREALPROJECT_API AGrid : public AActor {
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		AGrid();

		UPROPERTY(EditAnywhere, Category = "Grid") TSubclassOf<AActor> CellActorClass;
		UPROPERTY(EditAnywhere, Category = "Grid") int32 GridSize = 15;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
		virtual void OnConstruction(const FTransform& Transform) override;

	private:
		UFUNCTION() void GenerateGrid();

		UFUNCTION() void OnMouseLeaveGrid();

		UPROPERTY() TArray<AGridCell*> GridCells;
};