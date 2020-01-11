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
}

void ATDShooterPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Shoot", IE_Pressed, this, &ATDShooterPlayerController::OnShootPressed);
	InputComponent->BindAction("Shoot", IE_Released, this, &ATDShooterPlayerController::OnShootReleased);
	

}

//void ATDShooterPlayerController::OnResetVR()
//{
//	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
//}
//
//void ATDShooterPlayerController::MoveToMouseCursor()
//{
//	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
//	{
//		if (ATDShooterCharacter* MyPawn = Cast<ATDShooterCharacter>(GetPawn()))
//		{
//			if (MyPawn->GetCursorToWorld())
//			{
//				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
//			}
//		}
//	}
//	else
//	{
//		// Trace to see what is under the mouse cursor
//		FHitResult Hit;
//		GetHitResultUnderCursor(ECC_Visibility, false, Hit);
//
//		if (Hit.bBlockingHit)
//		{
//			// We hit something, move there
//			SetNewMoveDestination(Hit.ImpactPoint);
//		}
//	}
//}
//
//void ATDShooterPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	FVector2D ScreenSpaceLocation(Location);
//
//	// Trace to see what is under the touch location
//	FHitResult HitResult;
//	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
//	if (HitResult.bBlockingHit)
//	{
//		// We hit something, move there
//		SetNewMoveDestination(HitResult.ImpactPoint);
//	}
//}
//
//void ATDShooterPlayerController::SetNewMoveDestination(const FVector DestLocation)
//{
//	APawn* const MyPawn = GetPawn();
//	if (MyPawn)
//	{
//		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());
//
//		// We need to issue move command only if far enough in order for walk animation to play correctly
//		if ((Distance > 120.0f))
//		{
//			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
//		}
//	}
//}
//
void ATDShooterPlayerController::OnShootPressed()
{
	isShooting = true;
	
}

void ATDShooterPlayerController::OnShootReleased()
{
	isShooting = false;
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

