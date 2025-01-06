// Fill out your copyright notice in the Description page of Project Settings.

#include "MinionSkeleton.h"

void AMinionSkeleton::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 4;
    AttackRange = 1;
    Health = 50;
    Damage = 15;
    Critical = 5;
    TurnSpeed = 20;
}