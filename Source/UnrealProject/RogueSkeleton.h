#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "RogueSkeleton.generated.h"

UCLASS()
class UNREALPROJECT_API ARogueSkeleton : public AEnemyUnit {
	GENERATED_BODY()
	
	protected:
		virtual void BeginPlay() override;
};