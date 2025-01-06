// Fill out your copyright notice in the Description page of Project Settings.

#include "MageSkeleton.h"

void AMageSkeleton::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 4;
    AttackRange = 3;
    Health = 50;
    Damage = 25;
    Critical = 10;
    TurnSpeed = 22;
}