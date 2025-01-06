// // Fill out your copyright notice in the Description page of Project Settings.

// #pragma once

// #include "CoreMinimal.h"
// #include "GameFramework/Actor.h"
// #include "Materials/MaterialInstanceDynamic.h"
// #include "GridCell.generated.h"

// UCLASS()
// class UNREALPROJECT_API AGridCell : public AActor {
// 	GENERATED_BODY()
	
// 	public:	
// 		// Sets default values for this actor's properties
// 		AGridCell();
// 		// Called every frame
// 		virtual void Tick(float DeltaTime) override;

// 		UPROPERTY(EditAnywhere, Category = "GridCell") UStaticMesh* CellMesh;

// 		UPROPERTY(EditAnywhere, Category = "GridCell") UMaterialInterface* BaseMaterial;
		
// 		UFUNCTION() void SetInitialColor();
// 		UFUNCTION() void SetColor(FLinearColor Color);
// 		UFUNCTION() void SetAsObstacle();
// 		UFUNCTION() void PlaceUnit(TSubclassOf<APlayerUnit> UnitType);

// 	protected:
// 		// Called when the game starts or when spawned
// 		virtual void BeginPlay() override;

// 	private:
// 		UPROPERTY() UStaticMeshComponent* MeshComponent;

// 		UFUNCTION() void OnMouseOverCell(UPrimitiveComponent* OverComponent);
// 		UFUNCTION() void OnMouseLeaveCell(UPrimitiveComponent* OverComponent);
// 		UFUNCTION() void OnMouseClickOnCell(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed);
		
// 		UPROPERTY() UMaterialInstanceDynamic* DynamicMaterial;
// 		UPROPERTY() FLinearColor InitialColor;
// };

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GridCell.generated.h"

UENUM(BlueprintType)
enum class ECellState : uint8 {
    Empty,          // La case est vide et accessible
    Obstacle,       // La case est un obstacle invisible
    OccupiedHero,   // Case occupée par un héros
    OccupiedEnemy,  // Case occupée par un ennemi
    Highlighted     // La case est mise en surbrillance pour interaction
};

UCLASS()
class UNREALPROJECT_API AGridCell : public AActor {
    GENERATED_BODY()
    
public:    
    AGridCell();

    // Fonction appelée chaque frame (désactivée ici, mais utile pour futures mécaniques)
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "GridCell")
    UStaticMesh* CellMesh;

    UPROPERTY(EditAnywhere, Category = "GridCell")
    UMaterialInterface* BaseMaterial;

    UFUNCTION()
    void SetColor(FLinearColor Color);

    UFUNCTION()
    void SetAsObstacle();

    UFUNCTION()
    void SetState(ECellState NewState);

	UFUNCTION()
	bool IsEmpty();

	UFUNCTION()
	bool IsHighlighted();

	UPROPERTY()
	FLinearColor HoverColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UStaticMeshComponent* MeshComponent;

    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterial;

    UPROPERTY()
    FLinearColor InitialColor;

    ECellState CurrentState = ECellState::Empty; // État actuel de la cellule

    UFUNCTION()
    void OnMouseOverCell(UPrimitiveComponent* OverComponent);

    UFUNCTION()
    void OnMouseLeaveCell(UPrimitiveComponent* OverComponent);

    UFUNCTION()
    void OnMouseClickOnCell(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed);

    void UpdateColorByState();
};