#include "Grid.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "queue"
#include "vector"

AGrid::AGrid() {
	PrimaryActorTick.bCanEverTick = false;

	GridRoot = CreateDefaultSubobject<USceneComponent>(TEXT("GridRoot"));
	RootComponent = GridRoot;
}

void AGrid::BeginPlay() {
	Super::BeginPlay();

	// Generation de la grille
	GenerateGrid();
}

void AGrid::GenerateGrid() {
	UWorld* world = GetWorld();
	if(!world) {
		UE_LOG(LogTemp, Error, TEXT("World not found!"));
		return;
	}

	FVector ActorInitialLocation = GetActorLocation();
	FVector CurCellLocation = ActorInitialLocation;

	// Definition de la taille du tableau de cellules
	GridCells.SetNum(GridSize * GridSize);

	for(int32 i = 0; i < GridSize; i++) {
		// Position de la cellule actuelle en X
		CurCellLocation = ActorInitialLocation - FVector(i * CellOffsetX, 0.0f, 0.0f);

		for(int32 j = 0; j < GridSize; j++) {
			// Position de la cellule actuelle en Y
			CurCellLocation.Y = ActorInitialLocation.Y + j * CellOffsetY;

			if(i % 2 == 1) {
				// Decalage en quinconce pour les hexagones
				CurCellLocation.Y += 100.0f;
			}

			// Creation de la cellule
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

	// Detection des obstacles
	DetectObstacles(world);
}


void AGrid::DetectObstacles(UWorld* world) {
	TArray<AActor*> foundActors;

	// Recuperation de tous les obstacles presents dans le monde
	UGameplayStatics::GetAllActorsWithTag(world, FName("Obstacle"), foundActors);

	for(AGridCell* cell : GridCells) {
		FVector cellLocation = cell->GetActorLocation();

		for(AActor* obstacle : foundActors) {
			if(obstacle && obstacle->GetComponentsBoundingBox().Intersect(cell->GetComponentsBoundingBox())) {
				//Pour chaque cellule, si c'est un obstacle, on la defini comme tel
				cell->SetAsObstacle();
			}
		}
	}
}

TArray<AGridCell*> AGrid::GetCellsInRange(AGridCell* CurCell, int32 Range) {
    TArray<AGridCell*> cellsInRange;

	// Lambda fonction pour verifier si le chemin entre deux cellules est accessible
	auto isAccessible = [&](AGridCell* startCell, AGridCell* targetCell) {
		return IsPathAvailable(startCell, targetCell, Range);
	};
    
	// Convertit les coordonnees de la cellule actuelle en coordonnees Cubiques pour le calcul de la portee
	FVector startCube = OddRToCubic(CurCell->X, CurCell->Y);

	// Boucle a travers tous les voisins dans un rayon defini par la portee
	for(int32 dx = -Range; dx <= Range; dx++) { // Axe X
		for(int32 dy = FMath::Max(-Range, -dx - Range); dy <= FMath::Min(Range, -dx + Range); dy++) { // Axe Y
			// Calcul de l'axe Z pour un systeme de coordonnees cubiques
			int32 dz = -dx - dy;

			// Calcule les coordonnees cubiques du voisin en utilisant le decalage
			FVector neighborCube = startCube + FVector(dx, dy, dz);

			// Convertit les coordonnees cubiques en coordonnees du systeme de grille de l'hexagone
			int neighborX = neighborCube.X;
			int neighborY = neighborCube.Y + FMath::FloorToInt(neighborCube.X / 2);

			// Verifie si les coordonnees du voisin sont valides dans la grille
			if(neighborX >= 0 && neighborX < GridSize && neighborY >= 0 && neighborY < GridSize) {
				// Recupere la cellule voisine a partir de la grille
				AGridCell* neighborCell = GridCells[neighborX * GridSize + neighborY];

				// Si la cellule voisine existe et est vide, et si elle est accessible depuis la cellule actuelle
				if(neighborCell && neighborCell->IsEmpty() && isAccessible(CurCell, neighborCell)) {
					// Ajoute la cellule voisine dans le tableau des cellules accessibles
					cellsInRange.Add(neighborCell);
				}
			}
		}
	}

    return cellsInRange;
}

FVector AGrid::OddRToCubic(int32 X, int32 Y) {
	// Calcul de la coordonnee 'q' qui est simplement egale a 'X'
    int32 q = X;

	// Calcul de la coordonnee 'r'. Pour un systeme de coordonnees hexagonal "Odd-r", on ajuste la coordonnee Y en fonction de X
    int32 r = Y - floor(X / 2); // Le decalage de Y est base sur la division entiere de X par 2, ce qui est specifique au systeme "Odd-r"

	// Calcul de la coordonnee 's'. Elle est la somme negative de q et r pour garantir que q + r + s = 0, comme dans les coordonnees cubiques
    int32 s = -q - r;

    return FVector(q, r, s);
}

bool AGrid::IsPathAvailable(AGridCell* startCell, AGridCell* targetCell, int32 Range) {
	// Liste ouverte pour les cellules a explorer, qui va contenir les cellules en attente d'exploration
	TQueue<AGridCell*> openList;

	// Liste fermee pour les cellules deja explorees
    TSet<AGridCell*> closedList;

	// Liste des cellules actuellement en file d'attente, pour eviter de les ajouter plusieurs fois
	TSet<AGridCell*> inQueue;

	// Ajouter la cellule de depart a la liste ouverte et la marquer comme etant en file d'attente
    openList.Enqueue(startCell);

	// Suivi de la cellule de depart
    inQueue.Add(startCell);

	// La distance du point de depart a lui-même est 0
	startCell->DistanceFromStart = 0;

	// Calcul de la distance de Manhattan entre la cellule de depart et la cellule cible
	int32 closestDistance = ManhattanDistanceOddR(startCell->X, startCell->Y, targetCell->X, targetCell->Y);

    while(!openList.IsEmpty()) {
        AGridCell* currentCell;

		// Defilement de la prochaine cellule a explorer
        openList.Dequeue(currentCell);

		// Si la cellule courante est la cellule cible, on a trouve un chemin
        if(currentCell == targetCell) {
            return true;
        }

		// Ajouter la cellule actuelle a la liste fermee parce qu'elle a ete exploree
        closedList.Add(currentCell);

        // Parcours des voisins de la cellule courante
        for(AGridCell* neighborCell : GetNeighbors(currentCell)) {
			// Ignorer les voisins deja explores ou ceux qui ne sont pas accessibles
			if(closedList.Contains(neighborCell) || !neighborCell->IsEmpty()) {
				continue;
			}

			// Calcul de la nouvelle distance en ajoutant 1 a la distance du point de depart
			int32 newDistance = currentCell->DistanceFromStart + 1;

			// Si la nouvelle distance est dans la portee et que le voisin n'est pas deja dans la liste d'attente
			if(newDistance <= Range && !inQueue.Contains(neighborCell)) {
				// Mettre a jour la distance de la cellule voisine
				neighborCell->DistanceFromStart = newDistance;

				// Ajouter le voisin a la liste ouverte et a la liste des cellules en file d'attente
				openList.Enqueue(neighborCell);

				// Ajouter a la liste des cellules en attente
				inQueue.Add(neighborCell);
			}
        }
    }
	
	// Cible non atteignable dans la portee donnee
    return false;
}

int32 AGrid::ManhattanDistanceOddR(int32 x1, int32 y1, int32 x2, int32 y2) {
	// Convertir les coordonnees de la grille en coordonnees cubiques en utilisant le systeme de coordonnees Odd-R
	FVector cube1 = OddRToCubic(x1, y1);
    FVector cube2 = OddRToCubic(x2, y2);

    // Distance de Manhattan en coordonnees cubiques
    return FMath::Max(
        FMath::Abs(cube1.X - cube2.X), // Difference absolue sur l'axe X
        FMath::Max(
            FMath::Abs(cube1.Y - cube2.Y), // Difference absolue sur l'axe Y
            FMath::Abs(cube1.Z - cube2.Z) // Difference absolue sur l'axe Z
        )
    );
}

// Definition d'un comparateur pour les cellules de la grille
struct FGridCellComparator {
	// Surcharge de l'operateur () pour definir la logique de comparaison entre deux cellules de la grille
	bool operator()(const AGridCell* A, const AGridCell* B) const {
		// Comparer les cellules en fonction de leur coût total F-cost
        // Si le coût de A est plus grand que celui de B, retourner true, ce qui signifie que A sera prioritaire dans la file de priorite
		return A->GetFCost() > B->GetFCost();
	}
};

TArray<AGridCell*> AGrid::FindPath(AGridCell* StartCell, AGridCell* TargetCell) {
	TArray<AGridCell*> path;

	// File de priorite utilisee pour explorer les cellules avec le coût le plus bas
	std::priority_queue<AGridCell*, std::vector<AGridCell*>, FGridCellComparator> openList;

	// Ensembles utilises pour suivre les cellules en attente d'exploration (openSet) et les cellules deja explorees (closedList)
	TSet<AGridCell*> openSet;
	TSet<AGridCell*> closedList;

	// Initialisation des coûts pour la cellule de depart
	StartCell->GCost = 0;  // Coût pour atteindre la cellule de depart
	StartCell->HCost = ManhattanDistanceOddR(StartCell->X, StartCell->Y, TargetCell->X, TargetCell->Y); // Estimation du coût restant pour atteindre la cible
	StartCell->Parent = nullptr; // Pas de parent pour la cellule de depart

	// Ajouter la cellule de depart a la liste d'attente
	openList.push(StartCell);
	openSet.Add(StartCell);

	while(!openList.empty()) {
		// Recuperer la cellule avec le coût total le plus bas dans la file de priorite
		AGridCell* currentCell = openList.top();
		openList.pop();

		// Retirer la cellule de la liste d'attente
		openSet.Remove(currentCell);

		// Marquer la cellule comme exploree
		closedList.Add(currentCell);

		// Si la cellule actuelle est la cellule cible, on reconstruit le chemin
		if(currentCell == TargetCell) {
			// Remonter les parents pour reconstruire le chemin trouve
			while(currentCell) {
				// Ajouter la cellule au debut du chemin
				path.Insert(currentCell, 0);

				// On remonte au parent
				currentCell = currentCell->Parent;
			}

			// On retourne le chemin trouve
			return path;
		}

		// Explorer les voisins de la cellule actuelle
		for(AGridCell* neighborCell : GetNeighbors(currentCell)) {
			// Ignorer les voisins invalides, deja explores ou non accessibles
			if(!neighborCell || closedList.Contains(neighborCell) || !neighborCell->IsHighlighted() && !neighborCell->IsEmpty() && !(neighborCell == TargetCell)) {
				continue;
			}

			// Calculer le coût pour atteindre ce voisin
			int32 tentativeGCost = currentCell->GCost + 1;

			// Si le voisin n'est pas dans openSet ou si le nouveau coût est plus bas, on met a jour le voisin
			if(!openSet.Contains(neighborCell) || tentativeGCost < neighborCell->GCost) {
				// Mettre a jour le coût du chemin
				neighborCell->GCost = tentativeGCost;

				// Mettre a jour l'estimation du coût restant
				neighborCell->HCost = ManhattanDistanceOddR(neighborCell->X, neighborCell->Y, TargetCell->X, TargetCell->Y);

				// Definir la cellule actuelle comme parent
				neighborCell->Parent = currentCell;

				// Si le voisin n'est pas dans openSet, on l'ajoute a la liste d'attente
				if(!openSet.Contains(neighborCell)) {
					// Ajouter a la file de priorite
					openList.push(neighborCell);

					// Ajouter a openSet
					openSet.Add(neighborCell);
				}
			}
		}
	}

	// Si aucune solution n'a ete trouvee, on retourne un chemin vide
	return path;
}

TArray<AGridCell*> AGrid::GetNeighbors(AGridCell* Cell) {
	TArray<AGridCell*> neighbors;

	// Directions de deplacement possibles sur la grille hexagonale
	static const FVector Directions[6] = {
		FVector(0, -1, 1), FVector(1,-1, 0), // Haut gauche, Haut droite
		FVector(1, 0, -1), FVector(0, 1, -1), // Droite, Bas droite
		FVector(-1, 1, 0), FVector(-1, 0, 1) // Bas gauche, Gauche
	};

	// On boucle a travers toutes les directions possibles
	for(const FVector& direction : Directions) {
		// Convertir les coordonnees de la cellule courante en coordonnees cubiques
		FVector neighborCube = OddRToCubic(Cell->X, Cell->Y) + direction;

		// Calculer les coordonnees x et y du voisin
		int neighborX = neighborCube.X;
		int neighborY = neighborCube.Y + FMath::FloorToInt(neighborCube.X / 2);

		// Verifier si les coordonnees du voisin sont dans les limites de la grille
		if(neighborX >= 0 && neighborX < GridSize && neighborY >= 0 && neighborY < GridSize) {
			// Obtenir la cellule voisine dans la grille
			AGridCell* neighborCell = GridCells[neighborX * GridSize + neighborY];

			// Ajouter le voisin a la liste
			neighbors.Add(neighborCell);
		}
	}

	return neighbors;
}

void AGrid::HighlightCellsInRange(AGridCell* CurCell, int32 RemainingDisplacement) {
    TArray<AGridCell*> CellsInRange = GetCellsInRange(CurCell, RemainingDisplacement);

    for(AGridCell* cell : CellsInRange) {
		// Pour toutes les cellules dans la portée, on les marque comme "highlighted" et on passe la couleur du hover en vert
	    cell->SetState(ECellState::Highlighted);
		cell->HoverColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
    }
}

void AGrid::ResetHighlightedCells() {
    for(AGridCell* cell : GridCells) {
        if(cell->IsHighlighted()) {
			// Pour toutes les cellules "highlighted", on les marque comme "empty" et on passe la couleur du hover en rouge
            cell->SetState(ECellState::Empty);
			cell->HoverColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
        }
    }
}