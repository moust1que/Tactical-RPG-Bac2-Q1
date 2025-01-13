#include "Rogue.h"

void ARogue::BeginPlay() {
    Super::BeginPlay();

    // On initialise les statistiques du Rogue
    DisplacementRange = 6;
    AttackRange = 5;
    MaxHealth = 80;
    Health = MaxHealth;
    Damage = 25;
    TurnSpeed = 12;
}