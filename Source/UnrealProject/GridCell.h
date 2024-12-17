// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GridCell.generated.h"

UCLASS()
class UNREALPROJECT_API AGridCell : public AActor {
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		AGridCell();
		// Called every frame
		virtual void Tick(float DeltaTime) override;

		UPROPERTY(EditAnywhere, Category = "GridCell") UStaticMesh* CellMesh;

		UPROPERTY(EditAnywhere, Category = "GridCell") UMaterialInterface* BaseMaterial;
		
		UFUNCTION() void SetInitialColor();

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	private:
		UPROPERTY() UStaticMeshComponent* MeshComponent;

		UFUNCTION() void OnMouseOverCell(UPrimitiveComponent* OverComponent);
		UFUNCTION() void OnMouseLeaveCell(UPrimitiveComponent* OverComponent);
		UFUNCTION() void OnMouseClickOnCell(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed);
		
		UPROPERTY() UMaterialInstanceDynamic* DynamicMaterial;
		UPROPERTY() FLinearColor InitialColor;
};