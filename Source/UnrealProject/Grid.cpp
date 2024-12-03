// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

DEFINE_LOG_CATEGORY(LogGridCpp);

// Sets default values
AGrid::AGrid() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay() {
	Super::BeginPlay();

	if(!CellActorClass) {
		UE_LOG(LogGridCpp, Error, TEXT("Cell actor class not found! Please assign it in the editor."));
	}else {
		GenerateGrid();
	}
}

// Called every frame
void AGrid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AGrid::GenerateGrid() {
	UWorld* world = GEngine->GameViewport->GetWorld();
	const int32 cellSize = 100;

	for(int32 i = 0; i < GridSize; i++) {
		for(int32 j = 0; j < GridSize; j++) {
			int32 newIndexI = i - GridSize / 2;
			int32 newIndexJ = j - GridSize / 2;

			int32 posX = newIndexI * cellSize;
			int32 posY = newIndexJ * cellSize;

			FVector cellPosition = FVector(posX, posY, 0);

			FActorSpawnParameters spawnParams;
			world->SpawnActor<AActor>(CellActorClass, cellPosition, FRotator::ZeroRotator, spawnParams);
		}
	}
}