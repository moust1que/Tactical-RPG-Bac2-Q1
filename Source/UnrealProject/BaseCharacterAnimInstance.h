#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

UCLASS()
class UNREALPROJECT_API UBaseCharacterAnimInstance : public UAnimInstance {
	GENERATED_BODY()
	
	public:
		// Booleens permettant de jouer les animations des personnages
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsRunning;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsAttacking;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsHit;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsDead;
};