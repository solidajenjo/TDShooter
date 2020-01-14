// Fill out your copyright notice in the Description page of Project Settings.


#include "Shot.h"
#include "Components/StaticMeshComponent.h"
#include <TDShooter\Utils.h>
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>
#include <TDShooter\TDShooterPlayerController.h>
#include <TDShooter\EnemyAICharacter.h>


AShot::AShot()
{
	PrimaryActorTick.bCanEverTick = false;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemClass(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/ICE/P_Elemental_Proj_Impact_Ice.P_Elemental_Proj_Impact_Ice'"));
	particle = ParticleSystemClass.Object;		

}

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
	if (OtherComp->ComponentHasTag("Shot")) //Avoid shot pool particle spawning
		return;
	AEnemyAICharacter* enemy = Cast<AEnemyAICharacter>(OtherActor);
	if (enemy)
	{
		enemy->TakeDamage_(shotDamage);
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, GetActorLocation());	
	mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	SetActorLocation(SHOT_POOL_WAITING_ZONE);
}

void AShot::Go(const FVector& direction, const FVector& position)
{
	mesh->SetWorldLocation(position);
	mesh->SetPhysicsLinearVelocity(direction * shotSpeed);
	mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	FRotator newRot = UKismetMathLibrary::MakeRotFromXZ(direction, FVector(0.f, 0.f, 1.f));

	SetActorRotation(newRot);
}



