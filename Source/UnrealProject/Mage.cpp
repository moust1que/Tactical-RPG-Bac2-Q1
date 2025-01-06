// Fill out your copyright notice in the Description page of Project Settings.

#include "Mage.h"

void AMage::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 5;
    AttackRange = 4;
    Health = 70;
    Damage = 40;
    Critical = 15;
    TurnSpeed = 18;
}