// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovableTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMovableTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTAL1POINT5_API IMovableTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FVector GetLocation() = 0;
	virtual void SetLocation(FVector newLocation) = 0;

	virtual FVector GetVelocity() = 0;
	virtual void SetVelocity(FVector newVelocity) = 0;
	
	virtual FRotator GetRotation() = 0;
	virtual void SetRotation(FRotator newRotation) = 0;

};
