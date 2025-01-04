// Fill out your copyright notice in the Description page of Project Settings.

#include "FreeCamera.h"

// Sets default values
AFreeCamera::AFreeCamera() {
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 2000.0f;
    SpringArmComp->bDoCollisionTest = false;
    SpringArmComp->bUsePawnControlRotation = false;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	CalculateHexagonNormals();
}

// Called when the game starts or when spawned
void AFreeCamera::BeginPlay() {
	Super::BeginPlay();

	SpringArmComp->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f));
}

void AFreeCamera::CalculateHexagonNormals() {
	HexNormals.Empty();

	for(int32 i = 0; i < HexVertices.Num(); i++) {
		FVector2D vertexA = HexVertices[i];
		FVector2D vertexB = HexVertices[(i + 1) % HexVertices.Num()];

		FVector2D edge = vertexB - vertexA;
		FVector2D normal = FVector2D(-edge.Y, edge.X).GetSafeNormal();

		HexNormals.Add(normal);
	}
}

bool AFreeCamera::IsPointInHexagon(FVector2D Point) {
	float angleSum = 0.0f;

	for(int32 i = 0; i < HexVertices.Num(); i++) {
		FVector2D vertexA = HexVertices[i];
		FVector2D vertexB = HexVertices[(i + 1) % HexVertices.Num()];

		FVector2D vecA = (vertexA - Point).GetSafeNormal();
		FVector2D vecB = (vertexB - Point).GetSafeNormal();

		float dotProduct = FVector2D::DotProduct(vecA, vecB);
		float crossProduct = vecA.X * vecB.Y - vecA.Y * vecB.X;

		float angle = FMath::Atan2(crossProduct, dotProduct);
		angleSum += angle;
	}

	return FMath::Abs(angleSum) > KINDA_SMALL_NUMBER;
}

FVector2D AFreeCamera::ClampPawnToHexagon(FVector2D Point) {
	if(IsPointInHexagon(Point)) {
		return Point;
	}

	FVector2D closestPoint = Point;
	float minDistance = FLT_MAX;

	for(int32 i = 0; i < HexVertices.Num(); i++) {
		FVector2D vertexA = HexVertices[i];
		FVector2D vertexB = HexVertices[(i + 1) % HexVertices.Num()];

		FVector2D edge = vertexB - vertexA;
		float edgeLength = edge.Size();
		FVector2D edgeDirection = edge / edgeLength;

		float projection = FVector2D::DotProduct(Point - vertexA, edgeDirection);
		projection = FMath::Clamp(projection, 0.0f, edgeLength);

		FVector2D closestPointOnEdge = vertexA + edgeDirection * projection;

		float distance = FVector2D::DistSquared(Point, closestPointOnEdge);
		if(distance < minDistance) {
			minDistance = distance;
			closestPoint = closestPointOnEdge;
		}
	}

	return closestPoint;
}

void AFreeCamera::HandleFunctionCall(FName FunctionName, const FInputActionInstance& Instance, FName actionName) {
	if(UFunction* function = this->FindFunction(FunctionName)) {
		this->ProcessEvent(function, (void*)&Instance);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Function %s not found for action %s"), *FunctionName.ToString(), *actionName.ToString());
	}
}

void AFreeCamera::Move(const FInputActionInstance& Instance) {
	FVector2D axisValue2D = Instance.GetValue().Get<FVector2D>();
    
    if(axisValue2D.IsNearlyZero(0.3f)) return;

    FVector localMovement = FVector(axisValue2D.Y, axisValue2D.X, 0.0f);

	FRotator pawnRotation = GetActorRotation();

	FVector worldMovement = pawnRotation.RotateVector(localMovement) * CameraSpeed * (SpringArmComp->TargetArmLength / 500.0f) * GetWorld()->GetDeltaSeconds();
	FVector newLocation = GetActorLocation() + FVector(worldMovement.X, worldMovement.Y, 0.0f);

	FVector2D ClampedPosition = ClampPawnToHexagon(FVector2D(newLocation.X, newLocation.Y));
	newLocation.X = ClampedPosition.X;
	newLocation.Y = ClampedPosition.Y;

	SetActorLocation(newLocation, true);
}

void AFreeCamera::Rotate(const FInputActionInstance& Instance) {
	float axisValueX = Instance.GetValue().Get<FVector2D>().X;

    if(FMath::Abs(axisValueX) <= 0.3f) return;

    FRotator cameraRotation = FRotator(0.0f, axisValueX, 0.0f) * CameraRotationSpeed * GetWorld()->GetDeltaSeconds();

	FRotator newRotation = GetActorRotation() + cameraRotation;
	SetActorRotation(newRotation, ETeleportType::None);
}

void AFreeCamera::Zoom(const FInputActionInstance& Instance) {
	float axisValue = Instance.GetValue().Get<float>();

    if(FMath::Abs(axisValue) <= 0.3f) return;

    if(SpringArmComp) {
        float newLength = SpringArmComp->TargetArmLength - axisValue * CameraZoomSpeed * GetWorld()->GetDeltaSeconds();
        newLength = FMath::Clamp(newLength, CameraZoomMin, CameraZoomMax);

        SpringArmComp->TargetArmLength = newLength;
    }else {
        UE_LOG(LogTemp, Warning, TEXT("SpringArmComponent is not set."));
    }
}