// Fill out your copyright notice in the Description page of Project Settings.

#include "WarriorSkeleton.h"

void AWarriorSkeleton::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 4;
    AttackRange = 1;
    MaxHealth = 100;
    Health = MaxHealth;
    Damage = 30;
    TurnSpeed = 25;
}