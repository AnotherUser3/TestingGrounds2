// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UActorPool;

UCLASS()
class TESTINGGROUNDS2_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 0, int MaxSpawn = 1, float radius = 500, float MinScale = 1, float MaxScale = 1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);


private:

	bool CanSpawnAtLocation(FVector Location, float Radius);
	bool FindEmptyLocation(FVector& OutLocation, float radius);
	void SpawnActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale);
	void PositionNavMeshBoundsVolume();
	
	UActorPool* Pool;
	AActor* NavMeshBoundsVolume;
};
