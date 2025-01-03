// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGrid::AGrid() {
	PrimaryActorTick.bCanEverTick = false;

	GridRoot = CreateDefaultSubobject<USceneComponent>(TEXT("GridRoot"));
	RootComponent = GridRoot;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay() {
	Super::BeginPlay();

	if(!CellActorClass) {
		UE_LOG(LogTemp, Error, TEXT("Cell actor class not found! Please assign it in the editor."));
	}else {
		GenerateGrid();
	}
}

void AGrid::GenerateGrid() {
	UWorld* world = GetWorld();
	if(!world) {
		UE_LOG(LogTemp, Error, TEXT("World not found!"));
		return;
	}

	FVector ActorInitialLocation = GetActorLocation();
	FVector CurCellLocation = ActorInitialLocation;

	GridCells.SetNum(GridSize * GridSize);

	for(int32 i = 0; i < GridSize; i++) {
		CurCellLocation = ActorInitialLocation - FVector(i * CellSizeX, 0.0f, 0.0f);

		for(int32 j = 0; j < GridSize; j++) {
			CurCellLocation.Y = ActorInitialLocation.Y + j * CellSizeY;

			if(i % 2 == 1) {
				CurCellLocation.Y += 100.0f;
			}

			FActorSpawnParameters spawnParams;
			AGridCell* SpawnedCell = world->SpawnActor<AGridCell>(CellActorClass, CurCellLocation, FRotator::ZeroRotator, spawnParams);
			if(SpawnedCell) {
				GridCells[i * GridSize + j] = SpawnedCell;
				SpawnedCell->AttachToComponent(GridRoot, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}

	DetectObstacles(world);
}


void AGrid::DetectObstacles(UWorld* world) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsWithTag(world, FName("Obstacle"), foundActors);

	for(AGridCell* cell : GridCells) {
		FVector cellLocation = cell->GetActorLocation();

		for(AActor* obstacle : foundActors) {
			if(obstacle && obstacle->GetComponentsBoundingBox().Intersect(cell->GetComponentsBoundingBox())) {
				cell->SetAsObstacle();
			}
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