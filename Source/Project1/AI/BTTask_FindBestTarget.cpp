#include "BTTask_FindBestTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Project1/Combat/CombatComponent.h"

UBTTask_FindBestTarget::UBTTask_FindBestTarget()
{
	NodeName = "Find Best Target";

	// Filter to only allow Object keys
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindBestTarget, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_FindBestTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* Pawn = AIC->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UCombatComponent* Combat = Pawn->FindComponentByClass<UCombatComponent>();
	if (!Combat) return EBTNodeResult::Failed;

	AActor* BestTarget = Combat->FindBestTarget();

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	if (BestTarget)
	{
		BB->SetValueAsObject(TargetActorKey.SelectedKeyName, BestTarget);
		Combat->CurrentTarget = BestTarget;
		return EBTNodeResult::Succeeded;
	}

	BB->ClearValue(TargetActorKey.SelectedKeyName);
	Combat->CurrentTarget = nullptr;
	return EBTNodeResult::Failed;
}