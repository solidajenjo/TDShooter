// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include <TDShooter\EnemyAICharacter.h>
#include <Engine.h>
#include <TDShooter\Utils.h>

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBlueprint> enemyBP_(TEXT("Blueprint'/Game/TopDownCPP/Blueprints/BP_Enemy.BP_Enemy'"));
	enemyBP = (UClass*)enemyBP_.Object->GeneratedClass;
}

void AEnemySpawner::ReleaseEnemy(AEnemyAICharacter* enemy)
{
	enemy->SetActorLocation(enemy->waitingZone);

	enemyPool.push(enemy);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoints"), spawnPoints);

	if (enemyBP) {
		for (size_t i = 0; i < ENEMY_POOL_SIZE; ++i)
		{
			FVector waitingZone = ENEMY_POOL_WAITING_ZONE * (i + 1);
			AEnemyAICharacter* newEnemy = GetWorld()->SpawnActor<AEnemyAICharacter>(enemyBP, waitingZone, FRotator::ZeroRotator);
			newEnemy->waitingZone = waitingZone;
			newEnemy->GetCapsuleComponent()->SetEnableGravity(false);
			newEnemy->spawner = this;
			enemyPool.push(newEnemy);
		}
	}

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (spawnTimer > 0.f)
		spawnTimer -= DeltaTime;
	else if (!enemyPool.empty())
	{
		AEnemyAICharacter* nextEnemy = enemyPool.front();
		enemyPool.pop();		
		nextEnemy->SetActorLocation(spawnPoints[nextSpawn++]->GetActorLocation());
		nextEnemy->Reset();
		if (nextSpawn == spawnPoints.Num())
			nextSpawn = 0;

		timeBetweenSpawns -= timeBetweenSpawnsReductionRate;
		spawnTimer = timeBetweenSpawns;
	}
}

