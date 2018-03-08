// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float radius)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint;
		bool foundLocation = FindEmptyLocation(SpawnPoint, radius);
		if (foundLocation)
		{
			SpawnActor(ToSpawn, SpawnPoint);
		}
	}

}
bool ATile::FindEmptyLocation(FVector& OutLocation, float radius)
{
	FVector Min(0, -2000, 30);
	FVector Max(4000, 2000, 30);
	FBox Bounds(Min, Max);

	const int MAX_ATTEMPTS = 100;
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
void ATile::SpawnActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint)
{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
		SpawnedActor->SetActorRelativeLocation(SpawnPoint);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
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

	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
		DrawDebugSphere(
			GetWorld(),
			GlobalLocation,
			Radius,
			4,
			ResultColor,
			true
		);


	return !HasHit;
}