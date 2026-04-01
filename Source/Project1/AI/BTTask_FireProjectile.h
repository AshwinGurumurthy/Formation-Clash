#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FireProjectile.generated.h"

UCLASS()
class PROJECT1_API UBTTask_FireProjectile : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FireProjectile();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
