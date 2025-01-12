// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Math/UnrealMathUtility.h"
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
				SpawnedCell->X = i;
				SpawnedCell->Y = j;
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

TArray<AGridCell*> AGrid::GetCellsInRange(AGridCell* CurCell, int32 Range) {
    TArray<AGridCell*> cellsInRange;

	auto isAccessible = [&](AGridCell* startCell, AGridCell* targetCell) {
		return IsPathAvailable(startCell, targetCell, Range);
	};
    
	FVector startCube = OddRToCubic(CurCell->X, CurCell->Y);

	for(int32 dx = -Range; dx <= Range; dx++) {
		for(int32 dy = FMath::Max(-Range, -dx - Range); dy <= FMath::Min(Range, -dx + Range); dy++) {
			int32 dz = -dx - dy;

			FVector neighborCube = startCube + FVector(dx, dy, dz);

			int neighborX = neighborCube.X;
			int neighborY = neighborCube.Y + FMath::FloorToInt(neighborCube.X / 2);

			if(neighborX >= 0 && neighborX < GridSize && neighborY >= 0 && neighborY < GridSize) {
				AGridCell* neighborCell = GridCells[neighborX * GridSize + neighborY];

				if(neighborCell && neighborCell->IsEmpty() && isAccessible(CurCell, neighborCell)) {
					cellsInRange.Add(neighborCell);
				}
			}
		}
	}

    return cellsInRange;
}

FVector AGrid::OddRToCubic(int32 X, int32 Y) {
    int32 q = X; // Décalage pour les grilles en quinconce
    int32 r = Y - floor(X / 2);
    int32 s = -q - r;

    return FVector(q, r, s);
}

bool AGrid::IsPathAvailable(AGridCell* startCell, AGridCell* targetCell, int32 Range) {
	TQueue<AGridCell*> openList;  // Liste ouverte (cellules à explorer)
    TSet<AGridCell*> closedList;  // Liste fermée (cellules déjà explorées)
	TSet<AGridCell*> inQueue;

    openList.Enqueue(startCell);
    inQueue.Add(startCell); // Ajouter la cellule de départ au suivi
	startCell->DistanceFromStart = 0;

	int32 closestDistance = ManhattanDistanceOddR(startCell->X, startCell->Y, targetCell->X, targetCell->Y);

    while(!openList.IsEmpty()/* && currentMoves <= Range*/) {
        AGridCell* currentCell;
        openList.Dequeue(currentCell);

        if(currentCell == targetCell) {
            return true; // Cible atteinte dans la portée
        }

        closedList.Add(currentCell);

        // Parcours des voisins de la cellule courante
        for(AGridCell* neighborCell : GetNeighbors(currentCell)) {
			if(closedList.Contains(neighborCell) || !neighborCell->IsEmpty()) {
				continue;
			}

			int32 newDistance = currentCell->DistanceFromStart + 1;

			if(newDistance <= Range && !inQueue.Contains(neighborCell)) {
				neighborCell->DistanceFromStart = newDistance;
				openList.Enqueue(neighborCell);
				inQueue.Add(neighborCell); // Ajouter à la liste des cellules en attente
			}
        }
    }
	
    return false; // Cible non atteignable dans la portée donnée
}

int32 AGrid::ManhattanDistanceOddR(int32 x1, int32 y1, int32 x2, int32 y2) {
	FVector cube1 = OddRToCubic(x1, y1);
    FVector cube2 = OddRToCubic(x2, y2);

    // Distance Manhattan en coordonnées cubiques
    return FMath::Max(
        FMath::Abs(cube1.X - cube2.X),
        FMath::Max(
            FMath::Abs(cube1.Y - cube2.Y),
            FMath::Abs(cube1.Z - cube2.Z)
        )
    );
}

TArray<AGridCell*> AGrid::FindPath(AGridCell* StartCell, AGridCell* TargetCell) {
	TArray<AGridCell*> path;

	TArray<AGridCell*> openList;
	TSet<AGridCell*> closedList;

	StartCell->GCost = 0;
	StartCell->HCost = ManhattanDistanceOddR(StartCell->X, StartCell->Y, TargetCell->X, TargetCell->Y);
	StartCell->Parent = nullptr;

	openList.Add(StartCell);

	while(!openList.IsEmpty()) {
		openList.Sort([](const AGridCell& A, const AGridCell& B) {
			return A.GetFCost() < B.GetFCost();
		});

		AGridCell* currentCell = openList[0];
		
		openList.RemoveAt(0);
		closedList.Add(currentCell);

		if(currentCell == TargetCell) {
			while(currentCell) {
				path.Insert(currentCell, 0);
				currentCell = currentCell->Parent;
			}

			return path;
		}

		for(AGridCell* neighborCell : GetNeighbors(currentCell)) {
			if(!neighborCell || closedList.Contains(neighborCell) || !neighborCell->IsHighlighted() && !neighborCell->IsEmpty() && !(neighborCell == TargetCell)) {
				continue;
			}

			int32 tentativeGCost = currentCell->GCost + 1;

			if(!openList.Contains(neighborCell) || tentativeGCost < neighborCell->GCost) {
				neighborCell->GCost = tentativeGCost;
				neighborCell->HCost = ManhattanDistanceOddR(neighborCell->X, neighborCell->Y, TargetCell->X, TargetCell->Y);
				neighborCell->Parent = currentCell;

				if(!openList.Contains(neighborCell)) {
					openList.Add(neighborCell);
				}
			}
		}
	}

	return path;
}

TArray<AGridCell*> AGrid::GetNeighbors(AGridCell* Cell) {
	TArray<AGridCell*> neighbors;

	// Directions de déplacement possibles sur la grille hexagonale
	static const FVector Directions[6] = {
		FVector(0, -1, 1), FVector(1,-1, 0),
		FVector(1, 0, -1), FVector(0, 1, -1),
		FVector(-1, 1, 0), FVector(-1, 0, 1)
	};

	for(const FVector& direction : Directions) {
		FVector neighborCube = OddRToCubic(Cell->X, Cell->Y) + direction;
		int neighborX = neighborCube.X;
		int neighborY = neighborCube.Y + FMath::FloorToInt(neighborCube.X / 2);

		if(neighborX >= 0 && neighborX < GridSize && neighborY >= 0 && neighborY < GridSize) {
			AGridCell* neighborCell = GridCells[neighborX * GridSize + neighborY];
			neighbors.Add(neighborCell);
		}
	}

	return neighbors;
}

void AGrid::HighlightCellsInRange(AGridCell* CurCell, int32 RemainingDisplacement) {
    TArray<AGridCell*> CellsInRange = GetCellsInRange(CurCell, RemainingDisplacement);

    for(AGridCell* cell : CellsInRange) {
	    cell->SetState(ECellState::Highlighted);
		cell->HoverColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
    }
}

void AGrid::ResetHighlightedCells() {
    for(AGridCell* cell : GridCells) {
        if(cell->IsHighlighted()) {
            cell->SetState(ECellState::Empty);
			cell->HoverColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
        }
    }
}