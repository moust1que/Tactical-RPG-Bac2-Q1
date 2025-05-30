// Fill out your copyright notice in the Description page of Project Settings.

#include "RogueSkeleton.h"

void ARogueSkeleton::BeginPlay() {
    Super::BeginPlay();

    // On initialise les statistiques du Rogue Skeleton
    DisplacementRange = 5;
    AttackRange = 4;
    MaxHealth = 60;
    Health = MaxHealth;
    Damage = 20;
    TurnSpeed = 15;
}