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
    FName ActionName = Instance.GetSourceAction()->GetFName();

    if (ActionFunctionMapping.Contains(ActionName)) {
        FName FunctionName = ActionFunctionMapping[ActionName];

        if (UFunction* Function = this->FindFunction(FunctionName)) {
            ProcessFunction(Function, Instance);
        } else {
            UE_LOG(LogTemp, Warning, TEXT("Function %s not found for action %s"), *FunctionName.ToString(), *ActionName.ToString());
        }
    }
}

void ATacticalRPGPlayerController::ProcessFunction(UFunction* function, const FInputActionInstance& Instance) {
    struct FArguments {
        const FInputActionInstance& Instance;
    };

    FArguments args = {Instance};

    this->ProcessEvent(function, &args);
}

void ATacticalRPGPlayerController::CameraMove(const FInputActionInstance& Instance) {
    FVector2D axisValue2D = Instance.GetValue().Get<FVector2D>();
    UE_LOG(LogTemp, Log, TEXT("X: %f, Y: %f"), axisValue2D.X, axisValue2D.Y);
    
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
    float axisValue2DXAxis = Instance.GetValue().Get<FVector2D>().X;

    if(axisValue2DXAxis <= 0.3f && axisValue2DXAxis >= -0.3f) return;

    FRotator cameraRotation = FRotator(0.0f, axisValue2DXAxis, 0.0f) * CameraRotationSpeed * GetWorld()->GetDeltaSeconds();

    if(APawn* controlledPawn = GetPawn()) {
        FRotator newRotation = controlledPawn->GetActorRotation() + cameraRotation;
        controlledPawn->SetActorRotation(newRotation, ETeleportType::None);
    }
}

void ATacticalRPGPlayerController::CameraZoom(const FInputActionInstance& Instance) {
    float AxisValueY = Instance.GetValue().Get<FVector2D>().Y;

    // Vérifie si l'entrée est suffisamment significative pour être prise en compte
    if (FMath::Abs(AxisValueY) <= 0.3f) return;

    if (SpringArmComp) {
        // Calcul du nouveau zoom
        float NewLength = SpringArmComp->TargetArmLength - AxisValueY * CameraZoomSpeed * GetWorld()->GetDeltaSeconds();
        NewLength = FMath::Clamp(NewLength, CameraZoomMin, CameraZoomMax); // Limite le zoom entre des valeurs définies

        // Met à jour la longueur du SpringArm
        SpringArmComp->TargetArmLength = NewLength;

        UE_LOG(LogTemp, Log, TEXT("Updated TargetArmLength: %f"), NewLength);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("SpringArmComponent is not set."));
    }
}