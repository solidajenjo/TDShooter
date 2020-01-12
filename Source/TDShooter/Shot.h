// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shot.generated.h"


class ATDShooterPlayerController;

UCLASS()
class TDSHOOTER_API AShot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShot();

	void Go(const FVector& direction, const FVector& position);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void shotHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mesh = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Shot configuration")
		float shotSpeed = 100.f;
	
	
	ATDShooterPlayerController* owner = nullptr;

	UParticleSystem* Particle;

};
