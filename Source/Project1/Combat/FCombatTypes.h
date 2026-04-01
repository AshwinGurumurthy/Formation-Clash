#pragma once
#include "CoreMinimal.h"
#include "FCombatTypes.generated.h"

UENUM(BlueprintType)
enum class ECharType : uint8
{
	Melee    UMETA(DisplayName = "Melee"),
	Ranged   UMETA(DisplayName = "Ranged"),
	Flying   UMETA(DisplayName = "Flying")
};

UENUM(BlueprintType)
enum class EAttackPattern : uint8
{
	SingleTarget  UMETA(DisplayName = "Single Target"),
	Team          UMETA(DisplayName = "Team"),
	AoE           UMETA(DisplayName = "AoE")
};

USTRUCT(BlueprintType)
struct FUnitStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCooldown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharType CharType = ECharType::Ranged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackPattern AttackPattern = EAttackPattern::SingleTarget;
};
