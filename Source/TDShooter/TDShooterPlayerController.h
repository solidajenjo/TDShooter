// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <queue>

#include "TDShooterPlayerController.generated.h"

#define SHOT_POOL_SIZE 15
#define SHOT_POOL_WAITING_ZONE FVector(10000.f, 10000.f, 10000.f)

class ATDShooterCharacter;
class AShot;

UCLASS()
class ATDShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDShooterPlayerController();

protected:

	void BeginPlay() override;


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
	void ManageAiming();
	void ManageMovement();
	void Shoot();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Player movement") //This acts as gun cooldown also
		float rotationTime = .4f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Player movement")
		float moveSpeed = 600.f;

	ATDShooterCharacter* character = nullptr;

	FRotator targetRotation;
	FRotator originalRotation;
	float rotTimer = 0.f;

	FVector direction;

	bool isShooting = false;
	std::queue<AShot*> shotPool;
};


