// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUnit.h"

APlayerUnit::APlayerUnit() {
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 1000.0f;
    SpringArmComp->bDoCollisionTest = false;
    SpringArmComp->bUsePawnControlRotation = false;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
}

void APlayerUnit::BeginPlay() {
    Super::BeginPlay();
    SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f));
}