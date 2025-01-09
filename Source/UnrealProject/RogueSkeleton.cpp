// Fill out your copyright notice in the Description page of Project Settings.

#include "RogueSkeleton.h"

// ARogueSkeleton::ARogueSkeleton() {
//     DisplacementRange = 5;
//     AttackRange = 4;
//     Health = 60;
//     Damage = 20;
//     Critical = 10;
//     TurnSpeed = 15;
// }

void ARogueSkeleton::BeginPlay() {
    Super::BeginPlay();

    DisplacementRange = 5;
    AttackRange = 4;
    Health = 60;
    Damage = 20;
    Critical = 10;
    TurnSpeed = 15;
}

// void ARogueSkeleton::PostInitializeComponents() {
//     Super::PostInitializeComponents();
    
//     // Initialisation des variables dans un endroit plus sûr
//     DisplacementRange = 5;
//     AttackRange = 4;
//     Health = 60;
//     Damage = 20;
//     Critical = 10;
//     TurnSpeed = 15;
// }