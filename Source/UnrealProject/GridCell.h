#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// #include "Materials/MaterialInstanceDynamic.h"
#include "GridCell.generated.h"

// Enum des differents etats des cellules
UENUM(BlueprintType)
enum class ECellState : uint8 {
    Empty,          // La case est vide et accessible
    Obstacle,       // La case est un obstacle invisible
    OccupiedHero,   // Case occupee par un heros
    OccupiedEnemy,  // Case occupee par un ennemi
    Highlighted     // La case est mise en surbrillance pour interaction
};

UCLASS()
class UNREALPROJECT_API AGridCell : public AActor {
    GENERATED_BODY()
    
public:
    // Constructeur
    AGridCell();

    // Mesh de la cellule
    UPROPERTY(EditAnywhere, Category = "GridCell")
    UStaticMesh* CellMesh;

    // Materiel de la cellule
    UPROPERTY(EditAnywhere, Category = "GridCell")
    UMaterialInterface* BaseMaterial;

    // On change la couleur de la cellule
    UFUNCTION()
    void SetColor(FLinearColor Color);

    // On definit la cellule comme obstacle
    UFUNCTION()
    void SetAsObstacle();

    // On change l'etat de la cellule
    UFUNCTION()
    void SetState(ECellState NewState);

    // On verifie si la cellule est vide
	UFUNCTION()
	bool IsEmpty();

    // On verifie si la cellule est mise en surbrillance
	UFUNCTION()
	bool IsHighlighted();

    // Couleur de survol
	UPROPERTY()
	FLinearColor HoverColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

    // Coordonnees X et Y de la cellule
    int32 X, Y;

    // Grille a laquelle appartient la cellule
    class AGrid* Grid;

    // Variable de stockage de la distance depuis la cellule de depart lors dune recherche de chemin
    int32 DistanceFromStart;

    // On defini le personnages occupant la cellule
    void SetOccupant(class ABaseCharacter* Occupant);

    // On retourne le personnages occupant la cellule
    class ABaseCharacter* GetOccupant();

    // Valeurs de couts pour le pathfinding
    int32 GCost;
    int32 HCost;

    // On retourne le coût total
    UFUNCTION() int32 GetFCost() const;

    // Parent de la cellule dans la recherche d'un chemin
    AGridCell* Parent;

protected:
    virtual void BeginPlay() override;

private:
    // Mesh component de la cellule
    UPROPERTY()
    UStaticMeshComponent* MeshComponent;

    // Material instance de la cellule
    UPROPERTY()
    class UMaterialInstanceDynamic* DynamicMaterial;

    // Couleur initiale de la cellule
    UPROPERTY()
    FLinearColor InitialColor;

    //etat actuel de la cellule
    ECellState CurrentState = ECellState::Empty;

    // Fonction appelee lors du survol de la cellule
    UFUNCTION()
    void OnMouseOverCell(UPrimitiveComponent* OverComponent);

    // Fonction appelee lors de la sortie du survol de la cellule
    UFUNCTION()
    void OnMouseLeaveCell(UPrimitiveComponent* OverComponent);

    // Fonction appelee lors du clic de la souris sur la cellule
    UFUNCTION()
    void OnMouseClickOnCell(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed);

    // Met a jour la couleur de la cellule en fonction de son etat
    void UpdateColorByState();

    // Personnage occupant la cellule
    ABaseCharacter* OccupantCharacter;
};