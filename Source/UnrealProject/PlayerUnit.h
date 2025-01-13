#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerUnit.generated.h"

UCLASS()
class UNREALPROJECT_API APlayerUnit : public ABaseCharacter {
	GENERATED_BODY()

	public:
		// Constructeur
		APlayerUnit();

		// Fonction quand la souris clique sur la cellule
		UFUNCTION()
		void HandleCellClick(AGridCell* ClickedCell, bool bIsEnemy);

		// Fonction de prise de tour
		void TakeTurn() override;
};