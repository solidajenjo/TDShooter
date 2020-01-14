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
	tDShooterCharacter = (ATDShooterCharacter*)GetCharacter();
	for (size_t i = 0; i < SHOT_POOL_SIZE; ++i)
	{
		shotPool.push(GetWorld()->SpawnActor<AShot>(tDShooterCharacter->GetShotBP(), SHOT_POOL_WAITING_ZONE, FRotator::ZeroRotator));
	}
	isDead = false;
}

void ATDShooterPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	isDead = tDShooterCharacter->lifePercent <= 0.f;
	
	if (isDead)
		return;

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

	ManageMovement(DeltaTime);
	
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

	tDShooterCharacter->SetActorRotation(newRot);
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
		hitResult.ImpactPoint.Z = tDShooterCharacter->GetActorLocation().Z;
		FVector Forward = hitResult.ImpactPoint - tDShooterCharacter->GetActorLocation();
		targetRotation = UKismetMathLibrary::MakeRotFromXZ(Forward, FVector(0.f, 0.f, 1.f));
		originalRotation = GetPawn()->GetActorRotation();
	}	
}

void ATDShooterPlayerController::ManageMovement(float dt)
{
	direction.Normalize();
	tDShooterCharacter->GetCapsuleComponent()->SetPhysicsLinearVelocity(direction * moveSpeed);
	
	if (!isShooting && direction.Size() > 0.f) // look in move direction
	{
		FRotator rot = tDShooterCharacter->GetCapsuleComponent()->GetComponentRotation();		
		FRotator newRot = UKismetMathLibrary::MakeRotFromXZ(direction, FVector(0.f, 0.f, 1.f));

		newRot = FMath::Lerp(rot, newRot, dt * 10.f);

		GetPawn()->SetActorRotation(newRot);
	}
}

void ATDShooterPlayerController::Shoot()
{
	USceneComponent* shotOrigin = tDShooterCharacter->GetShotOrigin();
	AShot* shot = shotPool.front();
	shot->Go(shotOrigin->GetForwardVector(), shotOrigin->GetComponentLocation());
	shotPool.pop();
	shotPool.push(shot);
}

