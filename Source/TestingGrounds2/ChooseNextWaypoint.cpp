// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolComponent.h" 
#include "Runtime/Engine/Classes/Components/ActorComponent.h"



EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the patrol points
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrolComponent = ControlledPawn->FindComponentByClass<UPatrolComponent>();
	if (!ensure(PatrolComponent)) { return EBTNodeResult::Failed; }
	auto PatrolPoints = PatrolComponent->GetPatrolPoints();
	// Warn about empty patrol points
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A character is missing patrol points"));
		return EBTNodeResult::Failed;
	}
	

	// Set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	auto CurrentPoint = PatrolPoints[Index];
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, CurrentPoint);

	// Cycle waypoint index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	

	return EBTNodeResult::Succeeded;
}

