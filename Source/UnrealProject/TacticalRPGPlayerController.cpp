#include "TacticalRPGPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "FreeCamera.h"

void ATacticalRPGPlayerController::BeginPlay() {
    Super::BeginPlay();

    // parametrages du player controller
    bShowMouseCursor = true;
    bEnableMouseOverEvents = true;
    bEnableClickEvents = true;

    if(ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player)) {
        if(UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
            // Ajout du mapping context
            InputSubsystem->AddMappingContext(IMC_CameraControl.LoadSynchronous(), 0);
        }
    }
}

void ATacticalRPGPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    if(InputComponent) {
        // Initialisation de la gestion dynamique des inputs
        SetupInputHandling(InputComponent);
    }
}

void ATacticalRPGPlayerController::SetupInputHandling(UInputComponent* PlayerInputComponent) {
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if(!Input) return;

    for(UInputAction* action : InputActions) {
        if(action) {
            // Pour chaque action, on la lie a la fonction DynamicInputHandler
            Input->BindAction(action, ETriggerEvent::Triggered, this, &ATacticalRPGPlayerController::DynamicInputHandler);
        }
    }
}

void ATacticalRPGPlayerController::DynamicInputHandler(const FInputActionInstance& Instance) {
    // Cette fonction appelle la fonction reliee au nom de l'action dans ActionFunctionMapping
    FName actionName = Instance.GetSourceAction()->GetFName();

    if(ActionFunctionMapping.Contains(actionName)) {
        FName functionName = ActionFunctionMapping[actionName];

        if(AFreeCamera* freeCamera = Cast<AFreeCamera>(GetPawn())) {
            freeCamera->HandleFunctionCall(functionName, Instance, actionName);
        }
    }
}