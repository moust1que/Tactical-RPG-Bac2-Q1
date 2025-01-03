// Fill out your copyright notice in the Description page of Project Settings.

#include "GridCell.h"

// Sets default values
AGridCell::AGridCell() {
	PrimaryActorTick.bCanEverTick = false;

	SetActorEnableCollision(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(RootComponent);

    MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Activer uniquement les requêtes de collision
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); // Type d'objet
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Ignorer tout par défaut
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	MeshComponent->SetGenerateOverlapEvents(true);
    MeshComponent->OnBeginCursorOver.AddDynamic(this, &AGridCell::OnMouseOverCell);
	MeshComponent->OnEndCursorOver.AddDynamic(this, &AGridCell::OnMouseLeaveCell);
	bool bIsBound = MeshComponent->OnEndCursorOver.IsBound();
    MeshComponent->OnClicked.AddDynamic(this, &AGridCell::OnMouseClickOnCell);

	InitialColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// Called when the game starts or when spawned
void AGridCell::BeginPlay() {
	Super::BeginPlay();

	if (!CellMesh) {
        UE_LOG(LogTemp, Error, TEXT("CellMesh is null! Assign a valid mesh in the editor or Blueprint."));
        return;
    }
	MeshComponent->SetStaticMesh(CellMesh);
	MeshComponent->SetRelativeRotation(FRotator(0.0f, 30.0f, 0.0f));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->bUseDefaultCollision = true;

	if(BaseMaterial) {
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		if(DynamicMaterial) {
			MeshComponent->SetMaterial(0, DynamicMaterial);
			SetInitialColor();
		}
	}
}

// Called every frame
void AGridCell::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AGridCell::OnMouseOverCell(UPrimitiveComponent* OverComponent) {
	if (DynamicMaterial) {
        DynamicMaterial->SetVectorParameterValue(TEXT("CellColor"), FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
    }
}

void AGridCell::OnMouseClickOnCell(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed) {
	if (DynamicMaterial) {
        DynamicMaterial->SetVectorParameterValue(TEXT("CellColor"), FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
    }
}

void AGridCell::OnMouseLeaveCell(UPrimitiveComponent* OverComponent) {
	SetInitialColor();
}

void AGridCell::SetInitialColor() {
	if (DynamicMaterial) {
		DynamicMaterial->SetVectorParameterValue(TEXT("CellColor"), InitialColor);
	}
}

void AGridCell::SetAsObstacle() {
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DynamicMaterial->SetVectorParameterValue(TEXT("CellColor"), FLinearColor(0.2f, 0.2f, 0.2f, 1.0f));

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}