#include "BTTask_FireProjectile.h"
#include "AIController.h"
#include "Project1/Combat/CombatComponent.h"

UBTTask_FireProjectile::UBTTask_FireProjectile()
{
	NodeName = "Fire Projectile";
}

EBTNodeResult::Type UBTTask_FireProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* Pawn = AIC->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UCombatComponent* Combat = Pawn->FindComponentByClass<UCombatComponent>();
	if (!Combat) return EBTNodeResult::Failed;

	if (!Combat->bCanAttack || Combat->bIsDead) return EBTNodeResult::Failed;

	Combat->FireProjectile();
	return EBTNodeResult::Succeeded;
}