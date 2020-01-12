// Fill out your copyright notice in the Description page of Project Settings.


#include "Shot.h"
#include "Components/StaticMeshComponent.h"
#include <TDShooter\Utils.h>
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>
#include <TDShooter\TDShooterPlayerController.h>


// Sets default values
AShot::AShot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("ParticleSystem'/Game/TopDownCPP/ParticleSystems/PT_ShotExplosion.PT_ShotExplosion'"));
	Particle = ParticleSystemClass.Object;		

}

// Called when the game starts or when spawned
void AShot::BeginPlay()
{
	Super::BeginPlay();
	Go(GetActorForwardVector(), GetActorLocation());

	mesh->SetSimulatePhysics(true);
	mesh->SetEnableGravity(false);
	mesh->SetNotifyRigidBodyCollision(true);

	mesh->OnComponentHit.AddDynamic(this, &AShot::shotHit);
}

void AShot::shotHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation());	
	SetActorLocation(SHOT_POOL_WAITING_ZONE);
}

void AShot::Go(const FVector& direction, const FVector& position)
{
	mesh->SetWorldLocation(position);
	mesh->SetPhysicsLinearVelocity(direction * shotSpeed);
	FRotator newRot = UKismetMathLibrary::MakeRotFromXZ(direction, FVector(0.f, 0.f, 1.f));

	SetActorRotation(newRot);
}



