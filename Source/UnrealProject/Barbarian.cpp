// Fill out your copyright notice in the Description page of Project Settings.

#include "Barbarian.h"

void ABarbarian::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 5;
    AttackRange = 1;
    Health = 150;
    Damage = 50;
    Critical = 10;
    TurnSpeed = 30;
}