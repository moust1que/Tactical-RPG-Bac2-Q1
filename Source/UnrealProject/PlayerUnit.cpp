// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUnit.h"
#include "TacticalRPGGameMode.h"
#include "Kismet/GameplayStatics.h"

APlayerUnit::APlayerUnit() {
    bIsHero = true;
}

void APlayerUnit::TakeTurn() {
    CurDisplacementUsed = 0;
    bCanAttack = true;
    remainingDisplacement = DisplacementRange;
    Grid->HighlightCellsInRange(CurCell, DisplacementRange);
}

void APlayerUnit::HandleCellClick(AGridCell* ClickedCell, bool bIsEnemy) {
    if(AnimInstance->IsRunning || AnimInstance->IsAttacking) {
        return;
    }

    TArray<AGridCell*> path = Grid->FindPath(CurCell, ClickedCell);

    int32 pathLength = path.Num() - 1;

    int32 directDistance = Grid->ManhattanDistanceOddR(CurCell->X, CurCell->Y, ClickedCell->X, ClickedCell->Y);

    if(bIsEnemy) {
        if(directDistance <= AttackRange) {
            Attack(ClickedCell->GetOccupant());
        }else if(pathLength > remainingDisplacement && pathLength <= remainingDisplacement + AttackRange) {
            path.SetNum(pathLength - AttackRange + 1);
            MoveAlongPath(path);
            TargetToAttack = ClickedCell->GetOccupant();
        }else if(pathLength > remainingDisplacement + AttackRange) {
            path.SetNum(remainingDisplacement + 1);
            MoveAlongPath(path);
        }else if(pathLength <= remainingDisplacement) {
            path.SetNum(pathLength);
            MoveAlongPath(path);
            TargetToAttack = ClickedCell->GetOccupant();
        }
    }else {
        if(pathLength > remainingDisplacement) {
            path.SetNum(remainingDisplacement + 1);
            MoveAlongPath(path);
        }else {
            MoveAlongPath(path);
        }
    }

    remainingDisplacement = DisplacementRange - CurDisplacementUsed;
}