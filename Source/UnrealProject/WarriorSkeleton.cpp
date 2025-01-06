// Fill out your copyright notice in the Description page of Project Settings.

#include "WarriorSkeleton.h"

void AWarriorSkeleton::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 4;
    AttackRange = 1;
    Health = 100;
    Damage = 30;
    Critical = 5;
    TurnSpeed = 25;
}