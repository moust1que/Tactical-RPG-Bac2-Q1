// Fill out your copyright notice in the Description page of Project Settings.

#include "TacticalRPGGameMode.h"

ATacticalRPGGameMode::ATacticalRPGGameMode() {
    PlayerControllerClass = ATacticalRPGPlayerController::StaticClass();

    CurrentGameState = EGameState::Starting;
}

void ATacticalRPGGameMode::BeginPlay() {
    Super::BeginPlay();

    if(!Grid) {
        FActorSpawnParameters spawnParams;
        spawnParams.Owner = this;
        spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        FVector sapwnLocation = FVector(1190.0f, -3300.0f, 200.1f);

        Grid = GetWorld()->SpawnActor<AGrid>(GridRef, sapwnLocation, FRotator::ZeroRotator, spawnParams);

        GridCells = Grid->GridCells;
    }

    PlayerController = GetWorld()->GetFirstPlayerController();

    PlacingMenuWidget = CreateWidget<UUserWidget>(PlayerController, PlacingMenuWidgetClass);

    ShowPlacingMenu();

    SetGameState(EGameState::PlacingUnits);
}

void ATacticalRPGGameMode::SetGameState(EGameState NewGameState) {
    if(CurrentGameState != NewGameState) {
        CurrentGameState = NewGameState;
        HandleGameStateChange(NewGameState);
    }
}

void ATacticalRPGGameMode::HandleGameStateChange(EGameState NewGameState) {
    switch (NewGameState) {
        case EGameState::PlacingUnits:
            break;
        case EGameState::PlayerPhase:
            break;
        case EGameState::EnemyPhase:
            break;
        default:
            break;
    }
}

void ATacticalRPGGameMode::PlaceUnit(EUnitType UnitType) {
    HidePlacingMenu();

    switch(UnitType) {
        case EUnitType::Knight:
            CurrentUnitType = Knight;
            break;
        case EUnitType::Rogue:
            CurrentUnitType = Rogue;
            break;
        case EUnitType::Mage:
            CurrentUnitType = Mage;
            break;
        case EUnitType::Warrior:
            CurrentUnitType = Warrior;
            break;
        default:
            break;
    }

    bPlacingUnits = true;

    for(int32 i = 0; i < 15; i++) {
        for(int32 j = 0; j < 2; j++) {
            GridCells[i * 15 + j]->SetColor(FLinearColor(0.2f, 0.2f, 1.0f, 1.0f));
        }
    }

    // UnitsPlaced++;
    // if(UnitsPlaced < MaxUnits) {
    //     ShowPlacingMenu();
    // }
}

void ATacticalRPGGameMode::ShowPlacingMenu() {
    PlacingMenuWidget->AddToViewport();

    FInputModeUIOnly inputModeData;
    inputModeData.SetWidgetToFocus(PlacingMenuWidget->TakeWidget());
    PlayerController->SetInputMode(inputModeData);
}

void ATacticalRPGGameMode::HidePlacingMenu() {
    PlacingMenuWidget->RemoveFromParent();

    FInputModeGameAndUI inputModeData;
    PlayerController->SetInputMode(inputModeData);
}

void ATacticalRPGGameMode::HandleCellClick(AGridCell* ClickedCell) {
    if(bPlacingUnits && ClickedCell /*&& !ClickedCell->IsOccupied()*/) {
        ClickedCell->PlaceUnit(CurrentUnitType);

        bPlacingUnits = false;

        for(int32 i = 0; i < 15; i++) {
            for(int32 j = 0; j < 2; j++) {
                GridCells[i * 15 + j]->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
            }
        }

        UnitsPlaced++;
        if(UnitsPlaced < MaxUnits) {
            ShowPlacingMenu();
        }else {
            SetGameState(EGameState::PlayerPhase);
        }
    }
}