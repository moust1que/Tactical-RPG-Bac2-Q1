// Fill out your copyright notice in the Description page of Project Settings.

#include "Rogue.h"

void ARogue::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 6;
    AttackRange = 5;
    MaxHealth = 80;
    Health = MaxHealth;
    Damage = 25;
    TurnSpeed = 12;
}