// Fill out your copyright notice in the Description page of Project Settings.


#include "GelProjectileMovementComponent.h"

void UGelProjectileMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	UE_LOG(LogTemp, Error, TEXT("Handle Impact : %f.%f.%f"),Velocity.X, Velocity.Y, Velocity.Z);
}

bool UGelProjectileMovementComponent::HandleDeflection(FHitResult& Hit, const FVector& OldVelocity,
	const uint32 NumBounces, float& SubTickTimeRemaining)
{
	UE_LOG(LogTemp, Error, TEXT("Handle Deflection : %f.%f.%f"),Velocity.X, Velocity.Y, Velocity.Z);
	return false;
}
