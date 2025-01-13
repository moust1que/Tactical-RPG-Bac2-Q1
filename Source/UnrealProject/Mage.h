#pragma once

#include "CoreMinimal.h"
#include "PlayerUnit.h"
#include "Mage.generated.h"

UCLASS()
class UNREALPROJECT_API AMage : public APlayerUnit {
	GENERATED_BODY()
	
	protected:
		virtual void BeginPlay() override;
};