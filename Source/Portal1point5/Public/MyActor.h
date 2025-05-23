// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortableSurface.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class PORTAL1POINT5_API AMyActor : public AActor, public IPortableSurface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;


	//멤버변수
private:
	
	//UPROPERTY(EditAnywhere)
	//bool bPutPortal = false;

	//함수
public:

	virtual bool GetPortable() override;

};
