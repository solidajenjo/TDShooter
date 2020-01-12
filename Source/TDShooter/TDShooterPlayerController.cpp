// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TDShooterPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TDShooterCharacter.h"
#include "Engine/World.h"

#include <TDShooter\Utils.h>
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

ATDShooterPlayerController::ATDShooterPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATDShooterPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (rotTimer > 0.f) {
		rotTimer -= DeltaTime;
		ManageRotation();
	}
	else
	{
		isRotating = false;
		ManageShooting();
	}

	ManageMovement();
	
}

void ATDShooterPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Shoot", IE_Pressed, this, &ATDShooterPlayerController::OnShootPressed);
	InputComponent->BindAction("Shoot", IE_Released, this, &ATDShooterPlayerController::OnShootReleased);
	
	InputComponent->BindAxis("MoveX", this, &ATDShooterPlayerController::MoveX);
	InputComponent->BindAxis("MoveY", this, &ATDShooterPlayerController::MoveY);
	

}

void ATDShooterPlayerController::OnShootPressed()
{
	isShooting = true;
	
}

void ATDShooterPlayerController::OnShootReleased()
{
	isShooting = false;
}

void ATDShooterPlayerController::MoveX(float value)
{
	direction.Y = value;
}

void ATDShooterPlayerController::MoveY(float value)
{
	direction.X = value;
}

void ATDShooterPlayerController::ManageRotation()
{
	float alpha = rotTimer / rotationTime;
	FRotator newRot = FMath::Lerp(targetRotation, originalRotation, alpha);

	GetPawn()->SetActorRotation(newRot);
}

void ATDShooterPlayerController::ManageShooting()
{
	if (isRotating || !isShooting)
		return;
	rotTimer = rotationTime;
	isRotating = true;

	float locationX;
	float locationY;

	GetMousePosition(locationX, locationY);
	FVector2D mousePosition(locationX, locationY);
	FHitResult hitResult;
	const bool bTraceComplex = false;
	if (GetHitResultAtScreenPosition(mousePosition, ECC_Visibility, bTraceComplex, hitResult) == true)
	{
		hitResult.ImpactPoint.Z = GetPawn()->GetActorLocation().Z;
		FVector Forward = hitResult.ImpactPoint - GetPawn()->GetActorLocation();
		targetRotation = UKismetMathLibrary::MakeRotFromXZ(Forward, FVector(0.f, 0.f, 1.f));
		originalRotation = GetPawn()->GetActorRotation();
	}
}

void ATDShooterPlayerController::ManageMovement()
{
	direction.Normalize();
	GetCharacter()->GetCapsuleComponent()->SetPhysicsLinearVelocity(direction * moveSpeed);
	if (!isShooting)
	{
		FRotator rot = GetCharacter()->GetCapsuleComponent()->GetComponentRotation();		
		FRotator newRot = UKismetMathLibrary::MakeRotFromXZ(direction, FVector(0.f, 0.f, 1.f));

		newRot = FMath::Lerp(rot, newRot, 0.1f);

		GetPawn()->SetActorRotation(newRot);
	}
}

