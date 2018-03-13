// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinSpawningExtent = FVector(0, -2000, 30);
	MaxSpawningExtent = FVector(4000, 2000, 30);
	NavigationBoundsOffset = FVector(2000, 0, 0);
}

void ATile::SetPool(UActorPool* InPool)
{
	Pool = InPool;

	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));

	PositionNavMeshBoundsVolume();

}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool."), *GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}



void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, MinScale, MaxScale, radius, SpawnPositions);

	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		SpawnActor(ToSpawn, SpawnPosition);
	}

}

TArray<FSpawnPosition> ATile::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float MinScale, float MaxScale, float radius, TArray<FSpawnPosition> &SpawnPositions)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool foundLocation = FindEmptyLocation(SpawnPosition.Location, radius * SpawnPosition.Scale);
		if (foundLocation)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.0f, 180.0f);
			SpawnPositions.Add(SpawnPosition);
		}
	}
	return SpawnPositions;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float radius)
{
	FBox Bounds(MinSpawningExtent, MaxSpawningExtent);

	const int MAX_ATTEMPTS = 30;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}

	return false;
}
void ATile::SpawnActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
		SpawnedActor->SetActorRelativeLocation(SpawnPosition.Location);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedActor->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		SpawnedActor->SetActorScale3D(FVector(SpawnPosition.Scale));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Pool->Return(NavMeshBoundsVolume);

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	//FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	//	DrawDebugSphere(
	//		GetWorld(),
	//		GlobalLocation,
	//		Radius,
	//		4,
	//		ResultColor,
	//		true
	//	);

	return !HasHit;
}