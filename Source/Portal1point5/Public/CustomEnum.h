// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PORTAL1POINT5_API CustomEnum
{
public:
	CustomEnum();
	~CustomEnum();
};

UENUM()
enum class GelStatus : int8
{
	Conversion, //변환
	Repulsion,  //반발
	Propulsion, //추진
	Cleaning    //세척
};

UENUM()
enum class PlaneStatus : int8
{
	Default,	//기본 상태
	Conversion,	//변환 젤 동작 상태
	Repulsion,	//반발 젤 동작 상태
	Propulsion	//추진 젤 동작 상태
};