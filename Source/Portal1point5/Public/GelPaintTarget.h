// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomEnum.h"
#include "UObject/Interface.h"
#include "GelPaintTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGelPaintTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTAL1POINT5_API IGelPaintTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void PaintGel(GelStatus paintingGel) = 0;
};
