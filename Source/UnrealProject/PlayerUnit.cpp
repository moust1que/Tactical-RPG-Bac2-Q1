// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUnit.h"

APlayerUnit::APlayerUnit() {
    bIsHero = true;
}

void APlayerUnit::TakeTurn(AGrid* Grid) {
    HighlightCellsInRange(Grid);
}

void APlayerUnit::HighlightCellsInRange(AGrid* Grid) {
    TArray<AGridCell*> CellsInRange = Grid->GetCellsInRange(CurCell, DisplacementRange);

    for(AGridCell* cell : CellsInRange) {
        // if(cell->IsEmpty()) {
            cell->SetState(ECellState::Highlighted);
        // }
    }
}

void APlayerUnit::HandleCellClick(AGridCell* ClickedCell) {
    if(ClickedCell->IsEmpty()) {

    }
}