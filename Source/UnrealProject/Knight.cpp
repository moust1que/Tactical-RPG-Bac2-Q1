// Fill out your copyright notice in the Description page of Project Settings.

#include "Knight.h"

void AKnight::BeginPlay() {
    Super::BeginPlay();
    
    DisplacementRange = 4;
    AttackRange = 2;
    MaxHealth = 120;
    Health = MaxHealth;
    Damage = 35;
    TurnSpeed = 25;
}