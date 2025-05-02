// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomUnrealExtention.h"

#include "Kismet/KismetMathLibrary.h"

CustomUnrealExtention::CustomUnrealExtention()
{
}

CustomUnrealExtention::~CustomUnrealExtention()
{
}

FVector CustomUnrealExtention::TransformRelativeLocation(const FTransform& newTransform,
	const FTransform& baseTransform, const FVector& targetWorldLocation)
{
	//주어진 Transform을 기반으로 현재 대상의 local location를 구하고
	FVector baseLocalLocation = UKismetMathLibrary::InverseTransformLocation(baseTransform, targetWorldLocation);
	//이 local location을 실제 대상의 Transform을 기반으로 적용하여 world location 결정
	FVector realLocation = UKismetMathLibrary::TransformLocation(newTransform, baseLocalLocation);

	return realLocation;
}

FRotator CustomUnrealExtention::TransformRelativeRotation(const FTransform& newTransform,
	const FTransform& baseTransform, const FRotator& targetWorldRotation)
{
	//현재 회전의 전방(x축) Vector를 구한다.
	FVector baseForward = UKismetMathLibrary::GetForwardVector(targetWorldRotation);
	//주어진 Transform을 기반으로 하는 현재 회전의 전방(x축)의 local direction을 구한다.
	FVector baseLocalForward = UKismetMathLibrary::InverseTransformDirection(baseTransform, baseForward);
	
	//현재 회전의 상향(z축) Vector를 구한다. (추후 회전 설정 시 정확한 방향을 위해 두개의 좌표가 필요하기 때문)
	FVector baseUp = UKismetMathLibrary::GetUpVector(targetWorldRotation);
	//주어진 Transform을 기반으로 하는 현재 회전의 상향(z축)의 local direction을 구한다.
	FVector baseLocalUp = UKismetMathLibrary::InverseTransformDirection(baseTransform, baseUp);

	//이 local direction을 새로운 Transform을 기반으로 적용하여 world rotation 결정
	//이 local Forward와 Up에 대해서 각각, 대상의 Transform 기반 Direction을 계산
	FVector realForward = UKismetMathLibrary::TransformDirection(newTransform, baseLocalForward);
	FVector realUp = UKismetMathLibrary::TransformDirection(newTransform, baseLocalUp);
	//이 Forward와 Up Direction을 기반으로 회전 계산 (두 축을 바탕으로 (이 기능의 x는 전방, z는 상향이므로 최초 forward와 up을 전방, 상향에서 가져온 것을 대입하면 됨) 정확한 방향 설정)
	FRotator realRotation = UKismetMathLibrary::MakeRotFromXZ(realForward, realUp);
	
	return realRotation;
}
