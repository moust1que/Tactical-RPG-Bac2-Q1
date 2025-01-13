#include "Barbarian.h"

void ABarbarian::BeginPlay() {
    Super::BeginPlay();

    // On initialise les statistiques du Barbarian
    DisplacementRange = 5;
    AttackRange = 1;
    MaxHealth = 150;
    Health = MaxHealth;
    Damage = 50;
    TurnSpeed = 30;
}