// Fill out your copyright notice in the Description page of Project Settings.

#include "TacticalRPGPlayerController.h"

void ATacticalRPGPlayerController::BeginPlay() {
    Super::BeginPlay();

    bShowMouseCursor = true;

    if(ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player)) {
        if(UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
            if(!InputMappingContext.IsNull()) {
                InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 0);
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
    Input->BindAction(IA_CursorMove, ETriggerEvent::Triggered, this, &ATacticalRPGPlayerController::CursorMove);
}

void ATacticalRPGPlayerController::CursorMove(const FInputActionInstance& Instance) {
    UE_LOG(LogTemp, Warning, TEXT("Cursor Move"));
    FVector2D AxisValue2D = Instance.GetValue().Get<FVector2D>();
    UE_LOG(LogTemp, Warning, TEXT("%f %f"), AxisValue2D.X, AxisValue2D.Y);
}