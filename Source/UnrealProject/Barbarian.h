#pragma once

#include "CoreMinimal.h"
#include "PlayerUnit.h"
#include "Barbarian.generated.h"

UCLASS()
class UNREALPROJECT_API ABarbarian : public APlayerUnit {
	GENERATED_BODY()
	
	protected:
		virtual void BeginPlay() override;
};