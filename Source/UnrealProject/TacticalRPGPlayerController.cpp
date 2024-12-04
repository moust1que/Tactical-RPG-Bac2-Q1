// Fill out your copyright notice in the Description page of Project Settings.

#include "TacticalRPGPlayerController.h"

void ATacticalRPGPlayerController::BeginPlay() {
    Super::BeginPlay();

    // bShowMouseCursor = true;

    if(ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player)) {
        if(UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
            if(!IMC_CameraControl.IsNull()) {
                InputSystem->AddMappingContext(IMC_CameraControl.LoadSynchronous(), 0);
            }
        }
    }
}

void ATacticalRPGPlayerController::SetupInputComponent() {
    Super::SetupInputComponent(); // Appelle la version de base pour s'assurer que tout est correctement initialisé

    if (InputComponent) {
        SetupInputHandling(InputComponent); // Appelle votre méthode personnalisée
    }
}

void ATacticalRPGPlayerController::SetupInputHandling(UInputComponent* PlayerInputComponent) {
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    Input->BindAction(IA_CameraMove, ETriggerEvent::Triggered, this, &ATacticalRPGPlayerController::CameraMove);
}

void ATacticalRPGPlayerController::CameraMove(const FInputActionInstance& Instance) {
    UE_LOG(LogTemp, Warning, TEXT("Cursor Move"));
    FVector2D AxisValue2D = Instance.GetValue().Get<FVector2D>();
    UE_LOG(LogTemp, Warning, TEXT("%f %f"), AxisValue2D.X, AxisValue2D.Y);
}