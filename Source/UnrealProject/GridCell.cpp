#include "GridCell.h"
#include "TacticalRPGGameMode.h"
#include "PlayerUnit.h"

AGridCell::AGridCell() {
    PrimaryActorTick.bCanEverTick = false;

    // On active les collisions
    SetActorEnableCollision(true);

    // On definis les composants qui composent la cellule
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(RootComponent);

    // Parametrage des collisions et des events
    MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
    MeshComponent->SetGenerateOverlapEvents(true);
    MeshComponent->OnBeginCursorOver.AddDynamic(this, &AGridCell::OnMouseOverCell);
    MeshComponent->OnEndCursorOver.AddDynamic(this, &AGridCell::OnMouseLeaveCell);
    MeshComponent->OnClicked.AddDynamic(this, &AGridCell::OnMouseClickOnCell);

    InitialColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // Blanc par defaut
}

void AGridCell::BeginPlay() {
    Super::BeginPlay();

    // Parametrage de la cellule
    MeshComponent->SetStaticMesh(CellMesh);
	MeshComponent->SetRelativeRotation(FRotator(0.0f, 30.0f, 0.0f));

    if(BaseMaterial) {
        DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        if(DynamicMaterial) {
            // On applique l'instance du materiel
            MeshComponent->SetMaterial(0, DynamicMaterial);
            
            // On met a jour la couleur
            UpdateColorByState();
        }
    }
}

// Change l'etat de la cellule et met a jour la couleur
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

// Met a jour la couleur en fonction de l'etat actuel
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

// Fonction quand la souris survole la cellule
void AGridCell::OnMouseOverCell(UPrimitiveComponent* OverComponent) {
    if(IsEmpty() || IsHighlighted()) {
        // On met la couleur du hover si la cellules est vide ou mise en surbrillance
        SetColor(HoverColor);
    }
}

// Fonction quand la souris quitte la cellule
void AGridCell::OnMouseLeaveCell(UPrimitiveComponent* OverComponent) {
    // Retour a la couleur d'etat
    UpdateColorByState();
}

// Fonction quand la souris clique sur la cellule
void AGridCell::OnMouseClickOnCell(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed) {
    ATacticalRPGGameMode* GameMode = Cast<ATacticalRPGGameMode>(GetWorld()->GetAuthGameMode());

    if(GameMode->bPlacingUnits && CurrentState == ECellState::Highlighted) {
        // Si le GameMode est en mode placement de unite et que la cellule est mise en surbrillance, on notifie le GameMode que la cellule a ete cliquee
        GameMode->HandleCellClick(this);
    }else if(CurrentState == ECellState::Highlighted || CurrentState == ECellState::Empty || CurrentState == ECellState::OccupiedEnemy) {
        // Si la cellule est mise en surbrillance ou vide ou occupee par un ennemi, on notifie l'unite dont c'est le tour que la cellule a ete cliquee
        APlayerUnit* playerUnit = Cast<APlayerUnit>(GameMode->AllUnits[0]);

        if(playerUnit && (playerUnit->CurDisplacementUsed < playerUnit->DisplacementRange || playerUnit->bCanAttack)) {
            playerUnit->HandleCellClick(this, (CurrentState == ECellState::OccupiedEnemy ? true : false));
        }
    }
}

// Fonction pour modifier la couleur de la cellule
void AGridCell::SetColor(FLinearColor Color) {
    if (DynamicMaterial) {
        DynamicMaterial->SetVectorParameterValue(TEXT("CellColor"), Color);
    }
}

// Definit une cellule comme un obstacle
void AGridCell::SetAsObstacle() {
    SetState(ECellState::Obstacle);
    // On desactive la collision
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// On definit le personnages occupant la cellule
void AGridCell::SetOccupant(ABaseCharacter* Occupant) {
    OccupantCharacter = Occupant;
}

// On retourne le personnages occupant la cellule
ABaseCharacter* AGridCell::GetOccupant() {
    return OccupantCharacter;
}

// On retourne le cout total de la cellule
int32 AGridCell::GetFCost() const {
    return GCost + HCost;
}