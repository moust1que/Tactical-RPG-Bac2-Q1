#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyUnit.generated.h"

UCLASS()
class UNREALPROJECT_API AEnemyUnit : public ABaseCharacter {
	GENERATED_BODY()

	public:
		// Constructeur
		AEnemyUnit();

		// Fonction de prise de tour
		void TakeTurn() override;

		// Fonction de recherche du joueur le plus proche
		class APlayerUnit* FindClosestPlayer(TArray<ABaseCharacter*> AllUnits);
};