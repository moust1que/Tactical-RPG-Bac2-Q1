// Fill out your copyright notice in the Description page of Project Settings.

#include "RogueShootingRangeNPC.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

void URogueShootingRangeNPC::NativeBeginPlay() {
    Super::NativeBeginPlay();

    IsIdle = true;

    Delay(FMath::RandRange(5.0f, 10.0f), &URogueShootingRangeNPC::Reloading);
}

void URogueShootingRangeNPC::Delay(float time, void (URogueShootingRangeNPC::*function)()) {
    if(!function) return;

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, function);

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        TimerDelegate,
        time,
        false
    );
}

void URogueShootingRangeNPC::Reloading() {
    IsIdle = false;
    IsReloading = true;
    Delay(2.0f, &URogueShootingRangeNPC::Aiming);
}

void URogueShootingRangeNPC::Aiming() {
    IsReloading = false;
    IsAiming = true;
    Delay(2.0f, &URogueShootingRangeNPC::Shooting);
}

void URogueShootingRangeNPC::Shooting() {
    IsAiming = false;
    IsShooting = true;
    Delay(2.0f, &URogueShootingRangeNPC::Idle);
}

void URogueShootingRangeNPC::Idle() {
    IsShooting = false;
    IsIdle = true;
    Delay(FMath::RandRange(5.0f, 10.0f), &URogueShootingRangeNPC::Reloading);
}