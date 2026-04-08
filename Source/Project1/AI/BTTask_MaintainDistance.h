#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MaintainDistance.generated.h"

/**
 * Ranged unit movement: approach until in range, then backpedal if the target gets too close.
 * Keeps the unit at an optimal distance (between MinRangePct and MaxRangePct of AttackRange).
 */
UCLASS()
class PROJECT1_API UBTTask_MaintainDistance : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MaintainDistance();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** The unit will try to stay at this fraction of AttackRange (sweet spot) */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.5", ClampMax = "1.0"))
	float IdealRangePct = 0.85f;

	/** If the target is closer than this fraction of AttackRange, backpedal */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.2", ClampMax = "0.9"))
	float TooClosePct = 0.5f;
};
