// Fill out your copyright notice in the Description page of Project Settings.

#include "RogueSkeleton.h"

void ARogueSkeleton::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 5;
    AttackRange = 4;
    Health = 60;
    Damage = 20;
    Critical = 10;
    TurnSpeed = 15;
}