// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyUnit.h"
#include "TacticalRPGGameMode.h"
#include "PlayerUnit.h"
#include "Kismet/GameplayStatics.h"

AEnemyUnit::AEnemyUnit() {
    bIsHero = false;
}

void AEnemyUnit::TakeTurn() {
    if(AnimInstance->IsRunning || AnimInstance->IsAttacking) {
        return;
    }

    CurDisplacementUsed = 0;
    remainingDisplacement = DisplacementRange;
    bCanAttack = true;
    bWillAttack = true;

    TArray<ABaseCharacter*> allUnits = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->AllUnits;
    
    APlayerUnit* targetPlayer = FindClosestPlayer(allUnits);

    if(!targetPlayer) {
        UE_LOG(LogTemp, Warning, TEXT("No player found"));
        ATacticalRPGGameMode* gameMode = Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        gameMode->Defeat();
        //TODO Loose Screen
        return;
    }

    TArray<AGridCell*> path = Grid->FindPath(CurCell, targetPlayer->CurCell);

    int32 pathLength = path.Num() - 1;
    int32 directDistance = Grid->ManhattanDistanceOddR(CurCell->X, CurCell->Y, targetPlayer->CurCell->X, targetPlayer->CurCell->Y);

    // Logique pour attaquer ou se déplacer
    if (directDistance <= AttackRange) {
        // Si à portée d'attaque, attaquer directement
        Attack(targetPlayer);
    } else if (pathLength > remainingDisplacement && pathLength <= remainingDisplacement + AttackRange) {
        // Si hors de portée mais assez proche pour attaquer après déplacement
        path.SetNum(pathLength - AttackRange + 1);
        MoveAlongPath(path);
        TargetToAttack = targetPlayer;
    } else if (pathLength > remainingDisplacement + AttackRange) {
        // Si trop loin, déplacer jusqu'à la fin du déplacement possible
        path.SetNum(remainingDisplacement + 1);
        bWillAttack = false;
        MoveAlongPath(path);
    } else if (pathLength <= remainingDisplacement) {
        // Si peut se déplacer entièrement et attaquer
        path.SetNum(pathLength);
        MoveAlongPath(path);
        TargetToAttack = targetPlayer;
    }

    // Mettre à jour les déplacements restants
    remainingDisplacement = DisplacementRange - CurDisplacementUsed;

    // FTimerHandle EndTurnTimerHandle;
    // GetWorld()->GetTimerManager().SetTimer(EndTurnTimerHandle, [this]() {
    //     Cast<ATacticalRPGGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BattleUIWidget->EndTurn();
    // }, 6.0f, false);
}

APlayerUnit* AEnemyUnit::FindClosestPlayer(TArray<ABaseCharacter*> AllUnits) {
    APlayerUnit* closestPlayer = nullptr;
    float minDistance = FLT_MAX;

    FVector2D selfPosition = FVector2D(CurCell->X, CurCell->Y);
    
    for(ABaseCharacter* unit : AllUnits) {
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