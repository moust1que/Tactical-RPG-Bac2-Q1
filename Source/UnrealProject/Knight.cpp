#include "Knight.h"

void AKnight::BeginPlay() {
    Super::BeginPlay();
    
    // On initialise les statistiques du Knight
    DisplacementRange = 4;
    AttackRange = 1;
    MaxHealth = 120;
    Health = MaxHealth;
    Damage = 35;
    TurnSpeed = 25;
}