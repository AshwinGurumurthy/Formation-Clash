#include "BTTask_OrbitTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Project1/Combat/CombatComponent.h"

UBTTask_OrbitTarget::UBTTask_OrbitTarget()
{
	NodeName = "Orbit Target (Flying)";
	bNotifyTick = true;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_OrbitTarget, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_OrbitTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// Initialize the orbit angle based on where we currently are relative to target
	if (!bAngleInitialized)
	{
		FVector ToUnit = Pawn->GetActorLocation() - Target->GetActorLocation();
		CurrentAngleDeg = FMath::RadiansToDegrees(FMath::Atan2(ToUnit.Y, ToUnit.X));
		bAngleInitialized = true;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_OrbitTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	float OrbitRadius = AttackRange * OrbitRadiusPct;
	FVector TargetLoc = Target->GetActorLocation();
	float Distance = FVector::Dist(Pawn->GetActorLocation(), TargetLoc);

	if (Distance > AttackRange * 1.5f)
	{
		// Too far — close the gap first before orbiting
		AIC->MoveToActor(Target, OrbitRadius - 50.f);
		return;
	}

	// Advance the orbit angle
	CurrentAngleDeg += OrbitDegreesPerSecond * DeltaSeconds;
	if (CurrentAngleDeg > 360.f) CurrentAngleDeg -= 360.f;

	float AngleRad = FMath::DegreesToRadians(CurrentAngleDeg);
	FVector OrbitOffset = FVector(FMath::Cos(AngleRad) * OrbitRadius, FMath::Sin(AngleRad) * OrbitRadius, 0.f);
	FVector DesiredLocation = TargetLoc + OrbitOffset;

	// Move toward the next point on the orbit circle
	AIC->MoveToLocation(DesiredLocation, 30.f, false, true, false, true);
}
