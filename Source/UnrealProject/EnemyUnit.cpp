#include "EnemyUnit.h"
#include "TacticalRPGGameMode.h"
#include "PlayerUnit.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacterAnimInstance.h"
#include "Grid.h"

AEnemyUnit::AEnemyUnit() {
    // C'est un ennemi
    bIsHero = false;
}

void AEnemyUnit::TakeTurn() {
    // Si le personnage est en train de se deplacer ou d'attaquer, on ne fait rien
    if(AnimInstance->IsRunning || AnimInstance->IsAttacking) {
        return;
    }

    // Reinitialisation des variables pour permettre le bon deroulement du tour
    CurDisplacementUsed = 0;
    remainingDisplacement = DisplacementRange;
    bCanAttack = true;
    bWillAttack = true;

    // Tableau avec toutes les unites
    TArray<ABaseCharacter*> allUnits = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AllUnits;
    
    // Recherche du joueur le plus proche
    APlayerUnit* targetPlayer = FindClosestPlayer(allUnits);

    // Si aucun joueur trouve, on perd
    if(!targetPlayer) {
        ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        gameMode->Defeat();
        return;
    }

    // On recupere le chemin en direction de la target
    TArray<AGridCell*> path = Grid->FindPath(CurCell, targetPlayer->CurCell);

    // On calcule la distance directe et la longueur du chemin
    int32 pathLength = path.Num() - 1;
    int32 directDistance = Grid->ManhattanDistanceOddR(CurCell->X, CurCell->Y, targetPlayer->CurCell->X, targetPlayer->CurCell->Y);

    // Logique pour attaquer ou se deplacer
    if(directDistance <= AttackRange) {
        // Si la cible est a portee d'attaque, on attaque directement
        Attack(targetPlayer);
    }else if(pathLength > remainingDisplacement && pathLength <= remainingDisplacement + AttackRange) {
        // Si la cible est hors de portee de deplacement mais dans la portee d'attaque apres un deplacement plus petit que le nombre de deplacements restants
        path.SetNum(pathLength - AttackRange + 1);
        MoveAlongPath(path);
        TargetToAttack = targetPlayer;
    }else if(pathLength > remainingDisplacement + AttackRange) {
        // Si la cible est trop loin, on se rapproche
        path.SetNum(remainingDisplacement + 1);
        bWillAttack = false;
        MoveAlongPath(path);
    }else if(pathLength <= remainingDisplacement) {
        // Si on peut se deplacer entierement et attaquer
        path.SetNum(pathLength - AttackRange + 1);
        MoveAlongPath(path);
        TargetToAttack = targetPlayer;
    }

    // Mise a jour des deplacements restants
    remainingDisplacement = DisplacementRange - CurDisplacementUsed;
}

APlayerUnit* AEnemyUnit::FindClosestPlayer(TArray<ABaseCharacter*> AllUnits) {
    APlayerUnit* closestPlayer = nullptr;
    float minDistance = FLT_MAX;

    // On stocke notre position
    FVector2D selfPosition = FVector2D(CurCell->X, CurCell->Y);
    
    for(ABaseCharacter* unit : AllUnits) {
        // On recherche le joueur le plus proche
        if(!unit->bIsHero) {
            continue;
        }

        int32 distance = Grid->ManhattanDistanceOddR(CurCell->X, CurCell->Y, unit->CurCell->X, unit->CurCell->Y);

        if(distance < minDistance) {
            minDistance = distance;
            closestPlayer = Cast<APlayerUnit>(unit);
        }
    }

    return closestPlayer;
}