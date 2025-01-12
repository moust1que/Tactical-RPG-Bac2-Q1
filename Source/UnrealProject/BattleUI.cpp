// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUI.h"
#include "TurnOrderWidget.h"

void UBattleUI::CallUpdateTurnUI() {
    if(WBP_TurnOrder) {
        WBP_TurnOrder->UpdateTurnUI();
    }else {
        UE_LOG(LogTemp, Warning, TEXT("TurnOrderWidget is not set!"));
    }
}

void UBattleUI::EndTurn() {
    OnEndTurn.Broadcast();
}