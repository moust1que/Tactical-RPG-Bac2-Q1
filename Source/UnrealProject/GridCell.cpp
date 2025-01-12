// Fill out your copyright notice in the Description page of Project Settings.
#include "GridCell.h"
#include "TacticalRPGGameMode.h"

// Sets default values
AGridCell::AGridCell() {
    PrimaryActorTick.bCanEverTick = false;

    SetActorEnableCollision(true);

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(RootComponent);

    MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
    MeshComponent->SetGenerateOverlapEvents(true);

    MeshComponent->OnBeginCursorOver.AddDynamic(this, &AGridCell::OnMouseOverCell);
    MeshComponent->OnEndCursorOver.AddDynamic(this, &AGridCell::OnMouseLeaveCell);
    MeshComponent->OnClicked.AddDynamic(this, &AGridCell::OnMouseClickOnCell);

    InitialColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // Blanc par défaut
}

// Called when the game starts or when spawned
void AGridCell::BeginPlay() {
    Super::BeginPlay();

    if (!CellMesh) {
        UE_LOG(LogTemp, Error, TEXT("CellMesh is null! Assign a valid mesh in the editor or Blueprint."));
        return;
    }

    MeshComponent->SetStaticMesh(CellMesh);
	MeshComponent->SetRelativeRotation(FRotator(0.0f, 30.0f, 0.0f));

    if (BaseMaterial) {
        DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        if (DynamicMaterial) {
            MeshComponent->SetMaterial(0, DynamicMaterial);
            UpdateColorByState();
        }
    }
}

// Called every frame (non utilisé pour l'instant)
void AGridCell::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

// Change l'état de la cellule et met à jour la couleur
void AGridCell::SetState(ECellState NewState) {
    CurrentState = NewState;
    UpdateColorByState();
}

bool AGridCell::IsEmpty() {
	return CurrentState == ECellState::Empty;
}

bool AGridCell::IsHighlighted() {
	return CurrentState == ECellState::Highlighted;
}

// Met à jour la couleur en fonction de l'état actuel
void AGridCell::UpdateColorByState() {
    if (!DynamicMaterial) return;

    switch (CurrentState) {
        case ECellState::Empty:
            SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)); // Blanc
            break;
        case ECellState::Obstacle:
            SetColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f)); // Gris fonce
            MeshComponent->SetVisibility(false);
            break;
        case ECellState::OccupiedHero:
            SetColor(FLinearColor(0.0f, 0.0f, 1.0f, 1.0f)); // Bleu fonce
            break;
        case ECellState::OccupiedEnemy:
            SetColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)); // Rouge
            break;
        case ECellState::Highlighted:
            SetColor(FLinearColor(0.0f, 0.871f, 1.f, 1.0f)); // Bleu clair
            break;
    }
}

// Gestion des événements de la souris
void AGridCell::OnMouseOverCell(UPrimitiveComponent* OverComponent) {
    if (CurrentState == ECellState::Empty || CurrentState == ECellState::Highlighted) {
        SetColor(HoverColor);
    }
}

void AGridCell::OnMouseLeaveCell(UPrimitiveComponent* OverComponent) {
    UpdateColorByState(); // Retour à la couleur d'état
}

void AGridCell::OnMouseClickOnCell(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed) {
    ATacticalRPGGameMode* GameMode = Cast<ATacticalRPGGameMode>(GetWorld()->GetAuthGameMode());
    if(GameMode->bPlacingUnits && CurrentState == ECellState::Highlighted) {
        GameMode->HandleCellClick(this); // Notifie le GameMode
    }else if(CurrentState == ECellState::Highlighted || CurrentState == ECellState::Empty || CurrentState == ECellState::OccupiedEnemy) {
        APlayerUnit* playerUnit = Cast<APlayerUnit>(GameMode->AllUnits[0]);
        if(playerUnit && (playerUnit->CurDisplacementUsed < playerUnit->DisplacementRange || playerUnit->bCanAttack)) {
            playerUnit->HandleCellClick(this, (CurrentState == ECellState::OccupiedEnemy ? true : false));
        }
    }
}

// Change directement la couleur
void AGridCell::SetColor(FLinearColor Color) {
    if (DynamicMaterial) {
        DynamicMaterial->SetVectorParameterValue(TEXT("CellColor"), Color);
    }
}

// Définit une cellule comme un obstacle
void AGridCell::SetAsObstacle() {
    SetState(ECellState::Obstacle);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGridCell::SetOccupant(ABaseCharacter* Occupant) {
    OccupantCharacter = Occupant;
}

ABaseCharacter* AGridCell::GetOccupant() {
    return OccupantCharacter;
}

int32 AGridCell::GetFCost() const {
    return GCost + HCost;
}