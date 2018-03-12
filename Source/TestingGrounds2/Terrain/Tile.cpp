// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActorPool.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
		UE_LOG(LogTemp, Error, TEXT("Not enough actors in pool."));
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation());
}



void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float radius, float MinScale, float MaxScale)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		bool foundLocation = FindEmptyLocation(SpawnPoint, radius * RandomScale);
		if (foundLocation)
		{
			float RandomRotation = FMath::RandRange(-180.0f, 180.0f);
			SpawnActor(ToSpawn, SpawnPoint, RandomRotation, RandomScale);
		}
	}

}
bool ATile::FindEmptyLocation(FVector& OutLocation, float radius)
{
	FVector Min(0, -2000, 30);
	FVector Max(4000, 2000, 30);
	FBox Bounds(Min, Max);

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
void ATile::SpawnActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale)
{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
		SpawnedActor->SetActorRelativeLocation(SpawnPoint);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedActor->SetActorRotation(FRotator(0, Rotation, 0));
		SpawnedActor->SetActorScale3D(FVector(Scale));
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