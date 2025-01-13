#include "MageSkeleton.h"

void AMageSkeleton::BeginPlay() {
    Super::BeginPlay();

    // On initialise les statistiques du Mage Skeleton
    DisplacementRange = 4;
    AttackRange = 5;
    MaxHealth = 50;
    Health = MaxHealth;
    Damage = 25;
    TurnSpeed = 22;
}