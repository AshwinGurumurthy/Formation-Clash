#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_OrbitTarget.generated.h"

/**
 * Flying unit movement: circles around the target at attack range.
 * The continuous movement makes it hard for melee to pin down,
 * but the predictable radius lets ranged units land shots easily.
 */
UCLASS()
class PROJECT1_API UBTTask_OrbitTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_OrbitTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	/** Fraction of AttackRange to use as orbit radius */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.5", ClampMax = "1.0"))
	float OrbitRadiusPct = 0.8f;

	/** Degrees per second the unit orbits around the target */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "20.0", ClampMax = "180.0"))
	float OrbitDegreesPerSecond = 60.f;

private:
	float CurrentAngleDeg = 0.f;
	bool bAngleInitialized = false;
};
