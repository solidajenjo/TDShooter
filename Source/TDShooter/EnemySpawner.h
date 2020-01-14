// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <queue>
#include "EnemySpawner.generated.h"

#define ENEMY_POOL_SIZE 30
#define ENEMY_POOL_WAITING_ZONE FVector(20000.f, 20000.f, 20000.f)

class AEnemyAICharacter;

UCLASS()
class TDSHOOTER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
	TArray<AActor*> spawnPoints;
	std::queue<AEnemyAICharacter*> enemyPool;

	size_t nextSpawn = 0;

	float spawnTimer = 0.f;


public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	void ReleaseEnemy(AEnemyAICharacter* enemy);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawn parameters") 
		float timeBetweenSpawns = 3.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawn parameters")
		float timeBetweenSpawnsReductionRate = .1f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy blueprint")
		TSubclassOf<AEnemyAICharacter> enemyBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
