// Fill out your copyright notice in the Description page of Project Settings.

#include "TacticalRPGPlayerController.h"

void ATacticalRPGPlayerController::BeginPlay() {
    Super::BeginPlay();

    bShowMouseCursor = true;

    if (APawn* ControlledPawn = GetPawn()) {
        SpringArmComp = ControlledPawn->FindComponentByClass<USpringArmComponent>();
        if (!SpringArmComp) {
            UE_LOG(LogTemp, Warning, TEXT("SpringArmComponent not found on controlled pawn."));
        }
    }

    if(ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player)) {
        if(UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
            if(!IMC_CameraControl.IsNull()) {
                InputSubsystem->AddMappingContext(IMC_CameraControl.LoadSynchronous(), 0);
                CurrentInputMappingContext = IMC_CameraControl;
            } else {
                UE_LOG(LogTemp, Warning, TEXT("IMC_CameraControl is missing in BP_TacticalRPGPlayerController"));
            }
        }
    }
}

void ATacticalRPGPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    if (InputComponent) {
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

    if (ActionFunctionMapping.Contains(actionName)) {
        FName functionName = ActionFunctionMapping[actionName];

        if (UFunction* function = this->FindFunction(functionName)) {
            this->ProcessEvent(function, (void*)&Instance);
        } else {
            UE_LOG(LogTemp, Warning, TEXT("Function %s not found for action %s"), *functionName.ToString(), *actionName.ToString());
        }
    }
}

void ATacticalRPGPlayerController::CameraMove(const FInputActionInstance& Instance) {
    FVector2D axisValue2D = Instance.GetValue().Get<FVector2D>();
    
    if(axisValue2D.IsNearlyZero(0.3f)) return;

    FVector localMovement = FVector(axisValue2D.Y, axisValue2D.X, 0.0f);

    if(APawn* controlledPawn = GetPawn()) {
        FRotator pawnRotation = controlledPawn->GetActorRotation();

        FVector worldMovement = pawnRotation.RotateVector(localMovement) * CameraSpeed * GetWorld()->GetDeltaSeconds();
        FVector newLocation = controlledPawn->GetActorLocation() + FVector(worldMovement.X, worldMovement.Y, 0.0f);
        controlledPawn->SetActorLocation(newLocation, true);
    }
}

void ATacticalRPGPlayerController::CameraRotation(const FInputActionInstance& Instance) {
    float axisValueX = Instance.GetValue().Get<FVector2D>().X;

    if(FMath::Abs(axisValueX) <= 0.3f) return;

    FRotator cameraRotation = FRotator(0.0f, axisValueX, 0.0f) * CameraRotationSpeed * GetWorld()->GetDeltaSeconds();

    if(APawn* controlledPawn = GetPawn()) {
        FRotator newRotation = controlledPawn->GetActorRotation() + cameraRotation;
        controlledPawn->SetActorRotation(newRotation, ETeleportType::None);
    }
}

//! YValue == 0 ?????? WHYYYYYYYYY??????????
void ATacticalRPGPlayerController::CameraZoom(const FInputActionInstance& Instance) {
    float AxisValueY = Instance.GetValue().Get<FVector2D>().Y;
    UE_LOG(LogTemp, Log, TEXT("AxisValueY: %f"), AxisValueY);

    if (FMath::Abs(AxisValueY) <= 0.3f) return;

    if (SpringArmComp) {
        float NewLength = SpringArmComp->TargetArmLength - AxisValueY * CameraZoomSpeed * GetWorld()->GetDeltaSeconds();
        NewLength = FMath::Clamp(NewLength, CameraZoomMin, CameraZoomMax);

        SpringArmComp->TargetArmLength = NewLength;

        UE_LOG(LogTemp, Log, TEXT("Updated TargetArmLength: %f"), NewLength);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("SpringArmComponent is not set."));
    }
}