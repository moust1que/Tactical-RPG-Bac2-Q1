#include "MinionSkeleton.h"

void AMinionSkeleton::BeginPlay() {
    Super::BeginPlay();

    // On initialise les statistiques du Minion Skeleton
    DisplacementRange = 4;
    AttackRange = 1;
    MaxHealth = 50;
    Health = MaxHealth;
    Damage = 30;
    TurnSpeed = 20;
}