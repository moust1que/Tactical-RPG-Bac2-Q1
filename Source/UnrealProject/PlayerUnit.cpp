// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUnit.h"

APlayerUnit::APlayerUnit() {
    bIsHero = true;
}

void APlayerUnit::TakeTurn(AGrid* GridRef) {
    Grid = GridRef;
    CurDisplacementUsed = 0;
    bCanAttack = true;
    remainingDisplacement = DisplacementRange;
    Grid->HighlightCellsInRange(CurCell, DisplacementRange);
}

void APlayerUnit::HandleCellClick(AGridCell* ClickedCell, bool bIsEnemy) {
    if(AnimInstance->IsRunning || AnimInstance->IsAttacking) {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("PlayerUnit::HandleCellClick"));

    TArray<AGridCell*> path = Grid->FindPath(CurCell, ClickedCell, DisplacementRange);

    int32 pathLength = path.Num();

    if(bIsEnemy) {
        if(pathLength <= AttackRange) {
            Attack(ClickedCell->GetOccupant());
        }else if(pathLength > remainingDisplacement && pathLength <= remainingDisplacement + AttackRange) {
            path.SetNum(pathLength - AttackRange + 1);
            MoveAlongPath(path);
            TargetToAttack = ClickedCell->GetOccupant();
        }else if(pathLength > remainingDisplacement + AttackRange) {
            path.SetNum(remainingDisplacement + 1);
            MoveAlongPath(path);
        }else if(pathLength <= remainingDisplacement) {
            path.SetNum(pathLength - AttackRange + 1);
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