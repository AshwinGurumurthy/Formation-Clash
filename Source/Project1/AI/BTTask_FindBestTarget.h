#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindBestTarget.generated.h"

UCLASS()
class PROJECT1_API UBTTask_FindBestTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindBestTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// The blackboard key where we store the target
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
