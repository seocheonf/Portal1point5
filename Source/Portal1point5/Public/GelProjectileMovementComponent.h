// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GelProjectileMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PORTAL1POINT5_API UGelProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

protected:
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice = 0, const FVector& MoveDelta = FVector::ZeroVector) override;
	virtual bool HandleDeflection(FHitResult& Hit, const FVector& OldVelocity, const uint32 NumBounces, float& SubTickTimeRemaining) override;
};
