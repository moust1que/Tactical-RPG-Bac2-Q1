#include "RogueShootingRangeNPC.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

void URogueShootingRangeNPC::NativeBeginPlay() {
    Super::NativeBeginPlay();

    // on passe le personnage en mode idle
    IsIdle = true;

    // On lance un delay entre 5 et 10 secondes puis on appelle la fonction Reloading
    Delay(FMath::RandRange(5.0f, 10.0f), &URogueShootingRangeNPC::Reloading);
}

void URogueShootingRangeNPC::Delay(float time, void (URogueShootingRangeNPC::*function)()) {
    if(!function) return;

    // Creation d'un timer
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, function);

    // On lance le timer
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        TimerDelegate,
        time,
        false
    );
}

void URogueShootingRangeNPC::Reloading() {
    // On quitte l'etat idle et on entre dans l'etat de rechargement
    IsIdle = false;
    IsReloading = true;
    
    // On lance un delay de 2 secondes puis on appelle la fonction Aiming
    Delay(2.0f, &URogueShootingRangeNPC::Aiming);
}

void URogueShootingRangeNPC::Aiming() {
    // On quitte l'etat de rechargement et on entre dans l'etat de visee
    IsReloading = false;
    IsAiming = true;

    // On lance un delay de 2 secondes puis on appelle la fonction Shooting
    Delay(2.0f, &URogueShootingRangeNPC::Shooting);
}

void URogueShootingRangeNPC::Shooting() {
    // On quitte l'etat de vidsee et on entre dans l'etat de tir
    IsAiming = false;
    IsShooting = true;

    // On lance un delay de 2 secondes puis on appelle la fonction Idle
    Delay(2.0f, &URogueShootingRangeNPC::Idle);
}

void URogueShootingRangeNPC::Idle() {
    // On quitte l'etat de tir et on entre dans l'etat idle
    IsShooting = false;
    IsIdle = true;

    // On lance un delay entre 5 et 10 secondes puis on appelle la fonction Reloading
    Delay(FMath::RandRange(5.0f, 10.0f), &URogueShootingRangeNPC::Reloading);
}