#include "BTTask_MaintainDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Project1/Combat/CombatComponent.h"

UBTTask_MaintainDistance::UBTTask_MaintainDistance()
{
	NodeName = "Maintain Distance (Ranged)";
	bNotifyTick = true;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MaintainDistance, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_MaintainDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* Pawn = AIC->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UCombatComponent* Combat = Pawn->FindComponentByClass<UCombatComponent>();
	if (!Combat || Combat->bIsDead) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_MaintainDistance::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* Pawn = AIC->GetPawn();
	if (!Pawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UCombatComponent* Combat = Pawn->FindComponentByClass<UCombatComponent>();
	if (!Combat || Combat->bIsDead)
	{
		AIC->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* Target = BB ? Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName)) : nullptr;

	if (!Target)
	{
		AIC->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UCombatComponent* TargetCombat = Target->FindComponentByClass<UCombatComponent>();
	if (TargetCombat && TargetCombat->bIsDead)
	{
		AIC->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	float AttackRange = Combat->Stats.AttackRange;
	float Distance = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());
	float IdealDist = AttackRange * IdealRangePct;
	float TooCloseDist = AttackRange * TooClosePct;

	if (Distance < TooCloseDist)
	{
		// Too close — backpedal away from target
		FVector AwayDir = (Pawn->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
		FVector RetreatPoint = Target->GetActorLocation() + AwayDir * IdealDist;
		AIC->MoveToLocation(RetreatPoint, 50.f);
	}
	else if (Distance > AttackRange)
	{
		// Out of range — move closer to ideal distance
		float StopDistance = FMath::Max(IdealDist - 50.f, 100.f);
		AIC->MoveToActor(Target, StopDistance);
	}
	else
	{
		// In the sweet spot — stop and shoot
		AIC->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
