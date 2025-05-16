// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomEnum.h"
#include "GameFramework/Actor.h"
#include "GelGenerator.generated.h"

UCLASS()
class PORTAL1POINT5_API AGelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	class USceneComponent* BaseComp;
	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ArrowComp;
	
	UPROPERTY(EditAnywhere)
	GelStatus GelState;

	class AGel* GeneratedGel;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGel> OriginGel;

	UPROPERTY(EditAnywhere)
	float ShootSpeed = 500.f;
	UPROPERTY(EditAnywhere)
	float ShootInterval = 0.1f;

	FTimerHandle TimerHandle;
	
private:
	//In Constructor Function
	//컴포넌트 생성 및 계층 설정
	void ConstructionGenerateComponent();
	//컴포넌트 정보 초기화
	void ConstructionInitComponent();
	void CustomBeginPlay();
	void GenerateGel();

};
