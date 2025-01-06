// Fill out your copyright notice in the Description page of Project Settings.

#include "Knight.h"

void AKnight::BeginPlay() {
    Super::BeginPlay();
    
    DisplacementRange = 4;
    AttackRange = 1;
    Health = 120;
    Damage = 35;
    Critical = 5;
    TurnSpeed = 25;
}