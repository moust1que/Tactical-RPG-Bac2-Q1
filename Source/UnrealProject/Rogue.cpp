// Fill out your copyright notice in the Description page of Project Settings.

#include "Rogue.h"

void ARogue::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 6;
    AttackRange = 5;
    Health = 80;
    Damage = 25;
    Critical = 20;
    TurnSpeed = 12;
}