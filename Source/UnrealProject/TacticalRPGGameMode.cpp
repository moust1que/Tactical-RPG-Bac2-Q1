#include "TacticalRPGGameMode.h"
#include "TacticalRPGPlayerController.h"
#include "BattleUI.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "Grid.h"

ATacticalRPGGameMode::ATacticalRPGGameMode() {
    PlayerControllerClass = ATacticalRPGPlayerController::StaticClass();

    // On passe l'etat du jeu a Starting
    CurrentGameState = EGameState::Starting;
}

void ATacticalRPGGameMode::BeginPlay() {
    Super::BeginPlay();

    // Generation de la grille
    if(!Grid) {
        FActorSpawnParameters spawnParams;
        spawnParams.Owner = this;
        spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        FVector sapwnLocation = FVector(1190.0f, -3300.0f, 200.1f);

        Grid = GetWorld()->SpawnActor<AGrid>(GridRef, sapwnLocation, FRotator::ZeroRotator, spawnParams);

        GridCells = Grid->GridCells;
    }

    // Placement des unites ennemies sur la grille
    PlaceUnit(RogueSkeleton, false, GridCells[133]);
    PlaceUnit(MageSkeleton, false, GridCells[14]);
    PlaceUnit(MinionSkeleton, false, GridCells[208]);
    PlaceUnit(WarriorSkeleton, false, GridCells[103]);

    // On recupere le player controller
    PlayerController = GetWorld()->GetFirstPlayerController();

    // Creation du widget de placement des unites
    PlacingMenuWidget = CreateWidget<UUserWidget>(PlayerController, PlacingMenuWidgetClass);

    // On passe l'etat du jeu a PlacingUnits
    SetGameState(EGameState::PlacingUnits);
}

void ATacticalRPGGameMode::SetGameState(EGameState NewGameState) {
    if(CurrentGameState != NewGameState) {
        // S'il s'agit d'un nouvel etat de jeu, on appelle la fonction HandleGameStateChange et on change l'etat actuel
        CurrentGameState = NewGameState;
        HandleGameStateChange(NewGameState);
    }
}

void ATacticalRPGGameMode::HandleGameStateChange(EGameState NewGameState) {
    switch (NewGameState) {
        case EGameState::PlacingUnits:
            // On affiche le menu de placement
            ShowPlacingMenu();
            break;
        case EGameState::PlayPhase:
            // On lance le systeme de tour
            StartTurnSystem();
            break;
        default:
            break;
    }
}

void ATacticalRPGGameMode::SelectUnitToSpawn(EUnitType UnitType) {
    // On cache le menu de placement
    HidePlacingMenu();

    // On verifie le type de personnage et on l'assigne
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

    // On pace le booleen indiquant si le placement des personnages est en cours a true
    bPlacingUnits = true;

    for(int32 i = 0; i < 15; i++) {
        for(int32 j = 0; j < 2; j++) {
            if(GridCells[i * 15 + j]->IsEmpty()) {
                // on passe toutes les cellules vides des deux premieres lignes en "highlighted"
                GridCells[i * 15 + j]->SetState(ECellState::Highlighted);
                GridCells[i * 15 + j]->HoverColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
            }
        }
    }
}

void ATacticalRPGGameMode::ShowPlacingMenu() {
    // On affiche le widget
    PlacingMenuWidget->AddToViewport();

    // On met le focus sur le widget
    FInputModeUIOnly inputModeData;
    inputModeData.SetWidgetToFocus(PlacingMenuWidget->TakeWidget());
    PlayerController->SetInputMode(inputModeData);
}

void ATacticalRPGGameMode::HidePlacingMenu() {
    // On cache le widget
    PlacingMenuWidget->RemoveFromParent();

    // On met le focus sur le jeu
    FInputModeGameAndUI inputModeData;
    PlayerController->SetInputMode(inputModeData);
}

void ATacticalRPGGameMode::ShowBattleUI() {
    // On affiche le widget
    BattleUIWidget->AddToViewport();
}

void ATacticalRPGGameMode::HandleCellClick(AGridCell* ClickedCell) {
    if(bPlacingUnits && ClickedCell) {
        // Si le placement des personnages est en cours et qu'on a clique sur une cellule, on place le personnage
        PlaceUnit(CurrentUnitType, true, ClickedCell);

        // On passe le booleen indiquant si le placement des personnages est en cours a false
        bPlacingUnits = false;

        for(int32 i = 0; i < 15; i++) {
            for(int32 j = 0; j < 2; j++) {
                if(GridCells[i * 15 + j]->IsHighlighted()) {
                    // on enleve la surbrillance de toutes les cellules
                    GridCells[i * 15 + j]->SetState(ECellState::Empty);
                    GridCells[i * 15 + j]->HoverColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        }

        // On incremente le nombre de personnages place
        UnitsPlaced++;
        if(UnitsPlaced < MaxUnits) {
            // Si le nombre de personnages place est inferieur au maximum, on affiche le menu de placement
            ShowPlacingMenu();
        }else {
            // Sinon, on passe l'etat du jeu a PlayPhase
            SetGameState(EGameState::PlayPhase);
        }
    }
}

void ATacticalRPGGameMode::PlaceUnit(TSubclassOf<ABaseCharacter> UnitTypeToSpawn, bool bIsHero, AGridCell* CellToSpawnOn) {
    // On met a jour l'etat de la cellule en fonction de si c'est un ennemi ou un hero
    CellToSpawnOn->SetState(bIsHero ? ECellState::OccupiedHero : ECellState::OccupiedEnemy);

    // On place le personnage sur le centre de la cellule
    FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseCharacter* SpawnedUnit = GetWorld()->SpawnActor<ABaseCharacter>(UnitTypeToSpawn, CellToSpawnOn->GetActorLocation(), bIsHero ? FRotator::ZeroRotator : FRotator(0.0f, 180.0f, 0.0f), SpawnParams);

    // On enregistre la cellule sur laquelle le personnage est place dans le personnage
    SpawnedUnit->CurCell = CellToSpawnOn;
    // On defini le personnages occupant la cellule
    CellToSpawnOn->SetOccupant(SpawnedUnit);

    // On enregistre le personnage
    RegisterUnit(SpawnedUnit);
}

void ATacticalRPGGameMode::RegisterUnit(ABaseCharacter* Unit) {
    // On ajoute le personnage a la liste des personnages
    AllUnits.Add(Unit);
}

void ATacticalRPGGameMode::SortUnitsBySpeed() {
    // On trie la liste des personnages en fonction de leur vitesse
    AllUnits.Sort([](const ABaseCharacter& A, const ABaseCharacter& B) {
        return A.TurnProgress < B.TurnProgress;
    });
}

void ATacticalRPGGameMode::StartTurnSystem() {
    // On initialise la vitesse de tous les personnages
    for(ABaseCharacter* unit : AllUnits) {
        unit->TurnProgress = unit->TurnSpeed;
    }

    // On trie la liste des personnages en fonction de leur vitesse
    SortUnitsBySpeed();
    
    // On cree le widget de combat
    BattleUIWidget = CreateWidget<UBattleUI>(PlayerController, BattleUIWidgetClass);

    // On affiche le widget
    ShowBattleUI();

    // On lance le tour du premier personnage
    StartTurnForUnit(AllUnits[0]);
}

void ATacticalRPGGameMode::StartTurnForUnit(ABaseCharacter* Unit) {
    // On dit au personnage de commencer son tour
    Unit->TakeTurn();

    for(ABaseCharacter* unit : AllUnits) {
        if(unit == Unit) {
            continue;
        }
        
        // On met a jour la vitesse de tous les personnages sauf celui qui joue son tour
        unit->TurnProgress -= Unit->TurnProgress;
    }
    // On met a jour la vitesse du personnage qui joue son tour
    Unit->TurnProgress = Unit->TurnSpeed;
}

void ATacticalRPGGameMode::RemoveUnit(ABaseCharacter* Unit) {
    // On enleve le personnage de la liste
    AllUnits.Remove(Unit);
    // On met a jour l'UI
    BattleUIWidget->CallUpdateTurnUI();
}

void ATacticalRPGGameMode::Defeat() {
    // On affiche le widget de defaite
    DefeatWidget = CreateWidget<UUserWidget>(PlayerController, DefeatWidgetClass);
    DefeatWidget->AddToViewport();

    // On met le focus sur le widget
    FInputModeUIOnly inputModeData;
    inputModeData.SetWidgetToFocus(DefeatWidget->TakeWidget());
    PlayerController->SetInputMode(inputModeData);
}

void ATacticalRPGGameMode::Victory() {
    // On affiche le widget de victoire
    VictoryWidget = CreateWidget<UUserWidget>(PlayerController, VictoryWidgetClass);
    VictoryWidget->AddToViewport();

    // On met le focus sur le widget
    FInputModeUIOnly inputModeData;
    inputModeData.SetWidgetToFocus(VictoryWidget->TakeWidget());
    PlayerController->SetInputMode(inputModeData);
}