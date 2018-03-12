// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS2_API AInfiniteTerrainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:

	AInfiniteTerrainGameMode();
	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolumePool();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nav Mesh Pool")
		class UActorPool* NavMeshBoundsVolumePool;




private:

	void AddToPool(class ANavMeshBoundsVolume*);

	
};
