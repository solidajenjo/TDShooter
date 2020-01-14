// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAICharacter.generated.h"

class UCapsuleComponent;
class AEnemySpawner;
class UWidgetComponent;

UCLASS()
class TDSHOOTER_API AEnemyAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAICharacter();
	
	void TakeDamage_(float damage);

	void Reset();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Enemy stats")
		float totalLife = 100.f;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Enemy stats")
		float lifePercent = .1f;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Enemy stats")
		bool isActive = false;

	float lifeAmount = 0.f;

	AEnemySpawner* spawner = nullptr;
	FVector waitingZone;


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, CallInEditor)
		void SetDead();

	UFUNCTION(BlueprintCallable, CallInEditor)
		void AttackAction();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* attackTrigger;

};
