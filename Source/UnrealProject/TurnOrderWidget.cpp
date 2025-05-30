#include "TurnOrderWidget.h"
#include "TacticalRPGGameMode.h"
#include "Components/Image.h"
#include "BaseCharacter.h"

void UTurnOrderWidget::UpdateTurnUI() {
    for(int32 i = 0; i <= 7; i++) {
        int32 modIndex = i % GameMode->AllUnits.Num();

        // UTexture2D* SelectedImage

        FSlateBrush brush;
        brush.SetResourceObject(GameMode->AllUnits[modIndex]->PortraitTexture);
        brush.ImageSize = FVector2D(100.0f, 100.0f);

        TurnImages[i]->SetBrush(brush);
        TurnImages[i]->SetDesiredSizeOverride(FVector2D(100.0f, 100.0f));
    }
}