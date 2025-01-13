#include "WarriorSkeleton.h"

void AWarriorSkeleton::BeginPlay() {
    Super::BeginPlay();

    // On initialise les statistiques du Warrior Skeleton
    DisplacementRange = 4;
    AttackRange = 1;
    MaxHealth = 100;
    Health = MaxHealth;
    Damage = 40;
    TurnSpeed = 25;
}