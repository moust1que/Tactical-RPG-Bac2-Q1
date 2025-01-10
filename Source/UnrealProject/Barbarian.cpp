// Fill out your copyright notice in the Description page of Project Settings.

#include "Barbarian.h"

void ABarbarian::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 5;
    AttackRange = 1;
    MaxHealth = 150;
    Health = MaxHealth;
    Damage = 50;
    TurnSpeed = 30;
}