// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAICharacter.h"
#include "Components/CapsuleComponent.h"
#include <TDShooter\Utils.h>
#include <TDShooter\TDShooterCharacter.h>
#include <TDShooter\EnemySpawner.h>


AEnemyAICharacter::AEnemyAICharacter()
{
	attackTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShotOrigin"));
	attackTrigger->SetupAttachment(RootComponent);
}


void AEnemyAICharacter::TakeDamage_(float damage)
{
	lifeAmount -= damage;
	lifeAmount = FMath::Clamp(lifeAmount, 0.f, totalLife);
	lifePercent = lifeAmount / totalLife;
}

void AEnemyAICharacter::Reset()
{
	lifeAmount = totalLife;
	lifePercent = 1.f;
	isActive = true;
}


void AEnemyAICharacter::BeginPlay()
{
	Super::BeginPlay();
	Reset();
}

void AEnemyAICharacter::SetDead()
{
	spawner->ReleaseEnemy(this);
	isActive = false;
}

void AEnemyAICharacter::AttackAction()
{
	TArray<AActor*> overlappingActors;
	check(attackTrigger)
	attackTrigger->GetOverlappingActors(overlappingActors);

	for (AActor* a : overlappingActors) 
	{
		ATDShooterCharacter* player = Cast<ATDShooterCharacter>(a);

		if (player)
		{
			player->TakeDamage_(10.f);
		}
	}
}



