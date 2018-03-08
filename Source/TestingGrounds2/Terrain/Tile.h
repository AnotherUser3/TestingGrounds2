// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TESTINGGROUNDS2_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 0, int MaxSpawn = 1, float radius = 500);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool CanSpawnAtLocation(FVector Location, float Radius);
	bool FindEmptyLocation(FVector& OutLocation, float radius);
	void SpawnActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint);
	
};
