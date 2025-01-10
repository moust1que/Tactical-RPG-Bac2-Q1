// Fill out your copyright notice in the Description page of Project Settings.

#include "MinionSkeleton.h"

void AMinionSkeleton::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 4;
    AttackRange = 1;
    MaxHealth = 50;
    Health = MaxHealth;
    Damage = 15;
    TurnSpeed = 20;
}