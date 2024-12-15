// Fill out your copyright notice in the Description page of Project Settings.

#include "TacticalRPGPlayerController.h"

void ATacticalRPGPlayerController::BeginPlay() {
    Super::BeginPlay();

    bShowMouseCursor = true;

    if(ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player)) {
        if(UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
            if(!IMC_CameraControl.IsNull()){
                InputSubsystem->AddMappingContext(IMC_CameraControl.LoadSynchronous(), 0);
                CurrentInputMappingContext = IMC_CameraControl;
            }else {
                UE_LOG(LogTemp, Warning, TEXT("IMC_CameraControl is missing in BP_TacticalRPGPlayerController"));
            }
        }
    }
}

void ATacticalRPGPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    if(InputComponent) {
        SetupInputHandling(InputComponent);
    }
}

void ATacticalRPGPlayerController::SetupInputHandling(UInputComponent* PlayerInputComponent) {
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if(!Input) return;

    for(UInputAction* action : InputActions) {
        if(action) {
            Input->BindAction(action, ETriggerEvent::Triggered, this, &ATacticalRPGPlayerController::DynamicInputHandler);
        }
    }
}

void ATacticalRPGPlayerController::DynamicInputHandler(const FInputActionInstance& Instance) {
    FName actionName = Instance.GetSourceAction()->GetFName();

    if(ActionFunctionMapping.Contains(actionName)) {
        FName functionName = ActionFunctionMapping[actionName];

        if(AFreeCamera* freeCamera = Cast<AFreeCamera>(GetPawn())) {
            freeCamera->HandleFunctionCall(functionName, Instance, actionName);
        }
    }
}