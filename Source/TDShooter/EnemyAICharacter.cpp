// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAICharacter.h"
#include "Components/CapsuleComponent.h"
#include <TDShooter\Utils.h>
#include <TDShooter\TDShooterCharacter.h>

// Sets default values
AEnemyAICharacter::AEnemyAICharacter()
{
	attackTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShotOrigin"));
	attackTrigger->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void AEnemyAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAICharacter::AttackAction()
{
	TArray<AActor*> overlappingActors;
	attackTrigger->GetOverlappingActors(overlappingActors);

	for (AActor* a : overlappingActors) 
	{
		ATDShooterCharacter* player = Cast<ATDShooterCharacter>(a);

		if (player)
		{
			player->TakeDamage(10.f);
		}
	}
}



