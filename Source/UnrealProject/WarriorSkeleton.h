#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "WarriorSkeleton.generated.h"

UCLASS()
class UNREALPROJECT_API AWarriorSkeleton : public AEnemyUnit {
	GENERATED_BODY()
	
	protected:
		virtual void BeginPlay() override;
};