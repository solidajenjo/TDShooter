// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TDShooterPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TDShooterCharacter.h"
#include "Engine/World.h"

#include <TDShooter\Utils.h>
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>
#include <TDShooter\Shot.h>

ATDShooterPlayerController::ATDShooterPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATDShooterPlayerController::BeginPlay()
{
	character = (ATDShooterCharacter*)GetCharacter();
	for (size_t i = 0; i < SHOT_POOL_SIZE; ++i)
	{
		shotPool.push(GetWorld()->SpawnActor<AShot>(character->GetShotBP(), SHOT_POOL_WAITING_ZONE, FRotator::ZeroRotator));
	}
}

void ATDShooterPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (rotTimer > 0.f) {
		rotTimer -= DeltaTime;
		ManageRotation();
		if (rotTimer < 0.f)
		{
			Shoot();
		}
	}
	else if (isShooting)
	{
		ManageAiming();
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

	character->SetActorRotation(newRot);
}

void ATDShooterPlayerController::ManageAiming()
{
	
	rotTimer = rotationTime;

	float locationX;
	float locationY;

	GetMousePosition(locationX, locationY);
	FVector2D mousePosition(locationX, locationY);
	FHitResult hitResult;
	const bool bTraceComplex = false;
	if (GetHitResultAtScreenPosition(mousePosition, ECC_Visibility, bTraceComplex, hitResult) == true)
	{
		hitResult.ImpactPoint.Z = character->GetActorLocation().Z;
		FVector Forward = hitResult.ImpactPoint - character->GetActorLocation();
		targetRotation = UKismetMathLibrary::MakeRotFromXZ(Forward, FVector(0.f, 0.f, 1.f));
		originalRotation = GetPawn()->GetActorRotation();
	}	
}

void ATDShooterPlayerController::ManageMovement()
{
	direction.Normalize();
	character->GetCapsuleComponent()->SetPhysicsLinearVelocity(direction * moveSpeed);
	
	if (!isShooting && direction.Size() > 0.f) // look in move direction
	{
		FRotator rot = character->GetCapsuleComponent()->GetComponentRotation();		
		FRotator newRot = UKismetMathLibrary::MakeRotFromXZ(direction, FVector(0.f, 0.f, 1.f));

		newRot = FMath::Lerp(rot, newRot, 0.1f);

		GetPawn()->SetActorRotation(newRot);
	}
}

void ATDShooterPlayerController::Shoot()
{
	USceneComponent* shotOrigin = character->GetShotOrigin();
	AShot* shot = shotPool.front();
	shot->Go(shotOrigin->GetForwardVector(), shotOrigin->GetComponentLocation());
	shotPool.pop();
	shotPool.push(shot);
}

