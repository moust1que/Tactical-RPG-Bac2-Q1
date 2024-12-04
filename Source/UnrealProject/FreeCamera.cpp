// Fill out your copyright notice in the Description page of Project Settings.

#include "FreeCamera.h"

// Sets default values
AFreeCamera::AFreeCamera() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 1000.0f;
    SpringArmComp->bDoCollisionTest = false;
    SpringArmComp->bUsePawnControlRotation = false;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AFreeCamera::BeginPlay() {
	Super::BeginPlay();

	SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f));
}

// Called every frame
void AFreeCamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFreeCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}