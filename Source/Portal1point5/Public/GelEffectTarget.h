// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GelEffectTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGelEffectTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class GelEffectInfo
{
public:
	FVector targetVelocity = FVector::Zero();
	float targetAcceleration = 0.0f;
	float targetMoveSpeed = 0.0f;
	float targetJumpPower = 0.0f;
	float targetFriction = 0.0f;
};

class OriginGelEffectInfo
{
public:
	float originAcceleration = 0.0f;
	float originMoveSpeed = 0.0f;
	float originJumpPower = 0.0f;
	float originFriction = 0.0f;
};

class PORTAL1POINT5_API IGelEffectTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetMovementInfo(GelEffectInfo& outInfo, FVector& outBeforeFrameVelocity) = 0;
	virtual void SetMovementInfo(const GelEffectInfo& newInfo, const bool bReflection = false) = 0;
	virtual void GetOriginMovementInfo(OriginGelEffectInfo& outOriginInfo) = 0;
	
};