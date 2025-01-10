// Fill out your copyright notice in the Description page of Project Settings.

#include "MageSkeleton.h"

void AMageSkeleton::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 4;
    AttackRange = 3;
    MaxHealth = 50;
    Health = MaxHealth;
    Damage = 25;
    TurnSpeed = 22;
}