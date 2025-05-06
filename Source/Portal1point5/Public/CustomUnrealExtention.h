// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */


class PORTAL1POINT5_API CustomUnrealExtention
{
public:
	CustomUnrealExtention();
	~CustomUnrealExtention();


public:
	
	static FVector TransformRelativeLocation(const FTransform& newTransform, const FTransform& baseTransform, const FVector& targetWorldLocation);
	static FRotator TransformRelativeRotation(const FTransform& newTransform, const FTransform& baseTransform, const FRotator& targetWorldRotation);
	static FVector TransformRelativeDirection(const FTransform& newTransform, const FTransform& baseTransform, const FVector& targetWorldDirection);
	static FVector OrthographicProjection(const FVector& projectionVector, const FVector& normalVector);
};
