// Fill out your copyright notice in the Description page of Project Settings.

#include "Mage.h"

void AMage::BeginPlay() {
    Super::BeginPlay();

    // On initialise les statistiques du Mage
    DisplacementRange = 5;
    AttackRange = 4;
    MaxHealth = 70;
    Health = MaxHealth;
    Damage = 40;
    TurnSpeed = 18;
}