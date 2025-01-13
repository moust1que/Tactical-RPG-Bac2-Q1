#include "PlayerUnit.h"
#include "TacticalRPGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacterAnimInstance.h"
#include "Grid.h"

APlayerUnit::APlayerUnit() {
    // C'est un joueur
    bIsHero = true;
}

// Fonction de prise de tour
void APlayerUnit::TakeTurn() {
    // Reinitialisation des variables
    CurDisplacementUsed = 0;
    bCanAttack = true;
    remainingDisplacement = DisplacementRange;

    // On met en surbrillance les cellules accessibles
    Grid->HighlightCellsInRange(CurCell, DisplacementRange);
}

// Fonction de gestion du clic sur une cellule
void APlayerUnit::HandleCellClick(AGridCell* ClickedCell, bool bIsEnemy) {
    // Si le personnage est en train de se deplacer ou d'attaquer, on ne fait rien
    if(AnimInstance->IsRunning || AnimInstance->IsAttacking) {
        return;
    }

    // On recupere le chemin en direction de la target
    TArray<AGridCell*> path = Grid->FindPath(CurCell, ClickedCell);

    // On calcule la distance directe et la longueur du chemin
    int32 pathLength = path.Num() - 1;
    int32 directDistance = Grid->ManhattanDistanceOddR(CurCell->X, CurCell->Y, ClickedCell->X, ClickedCell->Y);

    if(bIsEnemy) {
        // Si c'est un ennemi
        if(directDistance <= AttackRange) {
            // Si la distance directe est inférieure ou égale à la portée d'attaque
            Attack(ClickedCell->GetOccupant());
        }else if(pathLength > remainingDisplacement && pathLength <= remainingDisplacement + AttackRange) {
            // Si la longueur du chemin est supérieure au déplacement restant et inférieure ou égale au deplacements restants cummules a la portée d'attaque
            path.SetNum(pathLength - AttackRange + 1);
            MoveAlongPath(path);
            TargetToAttack = ClickedCell->GetOccupant();
        }else if(pathLength > remainingDisplacement + AttackRange) {
            // Si la longueur du chemin est supérieure au déplacements restants cummules a la portée d'attaque
            path.SetNum(remainingDisplacement + 1);
            MoveAlongPath(path);
        }else if(pathLength <= remainingDisplacement) {
            // Si la longueur du chemin est inférieure ou égale au déplacement restant
            path.SetNum(pathLength - AttackRange + 1);
            MoveAlongPath(path);
            TargetToAttack = ClickedCell->GetOccupant();
        }
    }else {
        // Si c'est une case vide
        if(pathLength > remainingDisplacement) {
            // Si la longueur du chemin est supérieure au déplacement restant
            path.SetNum(remainingDisplacement + 1);
            MoveAlongPath(path);
        }else {
            // Tout autre cas
            MoveAlongPath(path);
        }
    }

    // Mise a jour des deplacements restants
    remainingDisplacement = DisplacementRange - CurDisplacementUsed;
}