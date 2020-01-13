// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDShooterCharacter.generated.h"

class AShot;
class UWidgetComponent;

UCLASS(Blueprintable)
class ATDShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDShooterCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	FORCEINLINE class USceneComponent* GetShotOrigin() { return shotOrigin; }
	FORCEINLINE class TSubclassOf<AShot> GetShotBP() { return ShotBP; }

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Player stats")
		float totalLife = 100.f;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Player stats")
		float lifePercent = .1f;

	float lifeAmount = 0.f;
	
	void BeginPlay() override;

	void TakeDamage(float damage);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* shotOrigin;

	UPROPERTY(EditDefaultsOnly, Category = "Shot blueprint")
		TSubclassOf<AShot> ShotBP;


};

