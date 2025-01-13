#pragma once

#include "CoreMinimal.h"
#include "PlayerUnit.h"
#include "Rogue.generated.h"
UCLASS()
class UNREALPROJECT_API ARogue : public APlayerUnit {
	GENERATED_BODY()

	protected:
		virtual void BeginPlay() override;
};