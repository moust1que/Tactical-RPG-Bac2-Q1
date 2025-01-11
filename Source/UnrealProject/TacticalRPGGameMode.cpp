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

    PlaceUnit(RogueSkeleton, false, GridCells[133]);
    // PlaceUnit(RogueSkeleton, false, GridCells[63]);
    // PlaceUnit(MageSkeleton, false, GridCells[14]);
    PlaceUnit(MageSkeleton, false, GridCells[63]);
    PlaceUnit(MinionSkeleton, false, GridCells[208]);
    PlaceUnit(WarriorSkeleton, false, GridCells[103]);

    PlayerController = GetWorld()->GetFirstPlayerController();

    PlacingMenuWidget = CreateWidget<UUserWidget>(PlayerController, PlacingMenuWidgetClass);

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
            ShowPlacingMenu();
            break;
        case EGameState::PlayPhase:
            StartTurnSystem();
            break;
        case EGameState::EndPhase:
            break;
        default:
            break;
    }
}

void ATacticalRPGGameMode::SelectUnitToSpawn(EUnitType UnitType) {
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
        case EUnitType::Barbarian:
            CurrentUnitType = Barbarian;
            break;
        default:
            break;
    }

    bPlacingUnits = true;

    for(int32 i = 0; i < 15; i++) {
        for(int32 j = 0; j < 2; j++) {
            if(GridCells[i * 15 + j]->IsEmpty()) {
                GridCells[i * 15 + j]->SetState(ECellState::Highlighted);
                GridCells[i * 15 + j]->HoverColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
            }
        }
    }
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

void ATacticalRPGGameMode::ShowBattleUI() {
    BattleUIWidget->AddToViewport();
}

void ATacticalRPGGameMode::HideBattleUI() {
    BattleUIWidget->RemoveFromParent();
}

void ATacticalRPGGameMode::HandleCellClick(AGridCell* ClickedCell) {
    if(bPlacingUnits && ClickedCell) {
        PlaceUnit(CurrentUnitType, true, ClickedCell);

        bPlacingUnits = false;

        for(int32 i = 0; i < 15; i++) {
            for(int32 j = 0; j < 2; j++) {
                if(GridCells[i * 15 + j]->IsHighlighted()) {
                    GridCells[i * 15 + j]->SetState(ECellState::Empty);
                    GridCells[i * 15 + j]->HoverColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        }

        UnitsPlaced++;
        if(UnitsPlaced < MaxUnits) {
            ShowPlacingMenu();
        }else {
            SetGameState(EGameState::PlayPhase);
        }
    }
}

void ATacticalRPGGameMode::PlaceUnit(TSubclassOf<ABaseCharacter> UnitTypeToSpawn, bool bIsHero, AGridCell* CellToSpawnOn) {
    CellToSpawnOn->SetState(bIsHero ? ECellState::OccupiedHero : ECellState::OccupiedEnemy);

    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseCharacter* SpawnedUnit = GetWorld()->SpawnActor<ABaseCharacter>(UnitTypeToSpawn, CellToSpawnOn->GetActorLocation(), bIsHero ? FRotator::ZeroRotator : FRotator(0.0f, 180.0f, 0.0f), SpawnParams);

    SpawnedUnit->CurCell = CellToSpawnOn;
    CellToSpawnOn->SetOccupant(SpawnedUnit);

    RegisterUnit(SpawnedUnit);
}

void ATacticalRPGGameMode::RegisterUnit(ABaseCharacter* Unit) {
    AllUnits.Add(Unit);
}

void ATacticalRPGGameMode::SortUnitsBySpeed() {
    AllUnits.Sort([](const ABaseCharacter& A, const ABaseCharacter& B) {
        return A.TurnProgress < B.TurnProgress;
    });
}

void ATacticalRPGGameMode::StartTurnSystem() {
    for(ABaseCharacter* unit : AllUnits) {
        unit->TurnProgress = unit->TurnSpeed;
    }

    SortUnitsBySpeed();
    
    BattleUIWidget = CreateWidget<UBattleUI>(PlayerController, BattleUIWidgetClass);

    ShowBattleUI();

    StartTurnForUnit(AllUnits[0]);
}

void ATacticalRPGGameMode::StartTurnForUnit(ABaseCharacter* Unit) {
    Unit->TakeTurn(Grid);

    for(ABaseCharacter* unit : AllUnits) {
        if(unit == Unit) {
            continue;
        }
        
        unit->TurnProgress -= Unit->TurnProgress;
    }
    Unit->TurnProgress = Unit->TurnSpeed;
}

void ATacticalRPGGameMode::RemoveUnit(ABaseCharacter* Unit) {
    AllUnits.Remove(Unit);
    // BattleUIWidget->UpdateTurnWidget();
    // BattleUIWidget->UpdateTurnUI();
    BattleUIWidget->CallUpdateTurnUI();
}