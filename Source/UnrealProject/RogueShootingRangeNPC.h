#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RogueShootingRangeNPC.generated.h"

UCLASS()
class UNREALPROJECT_API URogueShootingRangeNPC : public UAnimInstance {
	GENERATED_BODY()

	public:
		// Etats booleens
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsReloading = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsAiming = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsShooting = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsIdle = true;

	private:
		// Timer
		FTimerHandle TimerHandle;

		// Fonction Delay
		void Delay(float time, void (URogueShootingRangeNPC::*function)());

		// Fonctions de gestion des etats du NPC
		void Reloading();
		void Aiming();
		void Shooting();
		void Idle();

	protected:
		virtual void NativeBeginPlay() override;
};