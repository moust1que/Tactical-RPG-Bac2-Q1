#include "BattleUI.h"
#include "TurnOrderWidget.h"

void UBattleUI::CallUpdateTurnUI() {
    // On appel la fonction de mise a jour du widget
    if(WBP_TurnOrder) {
        WBP_TurnOrder->UpdateTurnUI();
    }else {
        UE_LOG(LogTemp, Warning, TEXT("TurnOrderWidget is not set!"));
    }
}

void UBattleUI::EndTurn() {
    // On declenche l'evenement de fin de tour
    OnEndTurn.Broadcast();
}