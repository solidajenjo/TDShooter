// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDShooterPlayerController.generated.h"

UCLASS()
class ATDShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDShooterPlayerController();

protected:

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface


	/** Input handlers*/
	void OnShootPressed();
	void OnShootReleased();
	void MoveX(float value);
	void MoveY(float value);

	void ManageRotation();
	void ManageShooting();
	void ManageMovement();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Player movement") //This acts as gun cooldown also
		float rotationTime = .2f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Player movement")
		float moveSpeed = 600.f;

	bool isRotating = false;
	FRotator targetRotation;
	FRotator originalRotation;
	float rotTimer = 0.f;

	FVector direction;

	bool isShooting = false;
};


