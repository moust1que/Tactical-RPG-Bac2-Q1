#pragma once

#include "CoreMinimal.h"
#include "EnemyUnit.h"
#include "MinionSkeleton.generated.h"

UCLASS()
class UNREALPROJECT_API AMinionSkeleton : public AEnemyUnit {
	GENERATED_BODY()
	
	protected:
		virtual void BeginPlay() override;
};