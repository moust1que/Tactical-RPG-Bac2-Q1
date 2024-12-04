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
    FVector2D AxisValue2D = Instance.GetValue().Get<FVector2D>();
    
    if(AxisValue2D.IsNearlyZero(0.01f)) return;

    FVector CameraMovement = FVector(AxisValue2D.Y, AxisValue2D.X, 0.0f) * CameraSpeed * GetWorld()->GetDeltaSeconds();

    if(APawn* ControlledPawn = GetPawn()) {
        FVector NewLocation = ControlledPawn->GetActorLocation() + CameraMovement;
        ControlledPawn->SetActorLocation(NewLocation, true);
    }
}