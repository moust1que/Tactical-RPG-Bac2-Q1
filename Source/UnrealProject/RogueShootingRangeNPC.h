// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RogueShootingRangeNPC.generated.h"

UCLASS()
class UNREALPROJECT_API URogueShootingRangeNPC : public UAnimInstance {
	GENERATED_BODY()

	public:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsReloading = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsAiming = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsShooting = false;
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags") bool IsIdle = true;

	private:
		FTimerHandle TimerHandle;

		void Delay(float time, void (URogueShootingRangeNPC::*function)());

		void Reloading();
		void Aiming();
		void Shooting();
		void Idle();

	protected:
		virtual void NativeBeginPlay() override;
};