// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

DEFINE_LOG_CATEGORY(LogGridCpp);

// Sets default values
AGrid::AGrid() {
	PrimaryActorTick.bCanEverTick = false;
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

void AGrid::GenerateGrid() {
	UWorld* world;
	if(GEngine && GEngine->GameViewport) {
		world = GEngine->GameViewport->GetWorld();
	}else {
		UE_LOG(LogGridCpp, Error, TEXT("Game viewport or GEngine not found!"));
		return;
	}

	FVector ActorInitialLocation = GetActorLocation();
	FVector CurCellLocation = ActorInitialLocation;

	for(int32 i = 0; i < GridSize; i++) {
		CurCellLocation = ActorInitialLocation - FVector(i * 170.0f, 0.0f, 0.0f);

		for(int32 j = 0; j < GridSize; j++) {
			CurCellLocation.Y = ActorInitialLocation.Y + j * 200.0f;

			if(i % 2 == 1) {
				CurCellLocation.Y += 100.0f;
			}

			FActorSpawnParameters spawnParams;
			world->SpawnActor<AActor>(CellActorClass, CurCellLocation, FRotator::ZeroRotator, spawnParams);
		}
	}
}

void AGrid::OnMouseLeaveGrid() {
	for(AGridCell* cell : GridCells) {
		if(cell) {
			cell->SetInitialColor();
		}
	}
}