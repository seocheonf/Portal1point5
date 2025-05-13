// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomEnum.h"
#include "MovableTarget.h"
#include "GameFramework/Actor.h"
#include "Gel.generated.h"

UCLASS()
class PORTAL1POINT5_API AGel : public AActor, public IMovableTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//인터페이스 함수
public:
	virtual FVector GetLocation() override;
	virtual void SetLocation(FVector newLocation) override;
	
	virtual FVector GetVelocity() override;
	virtual void SetVelocity(FVector newVelocity) override;

	virtual FRotator GetRotation() override;
	virtual void SetRotation(FRotator newRotation) override;


//멤버변수
private:
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere)
	class UGelProjectileMovementComponent* GelProjectileMovementComp;
	//class UProjectileMovementComponent* ProjectileMovementComp;

	//이전 속도
	FVector BeforeVelocity;

	UPROPERTY(EditAnywhere)
	GelStatus GelState = GelStatus::Cleaning;
	
//함수
private:
	
	//In Constructor Function
	//컴포넌트 생성 및 계층 설정
	void ConstructionGenerateComponent();
	//컴포넌트 정보 초기화
	void ConstructionInitComponent();

	void CustomBeginPlay();

	//충돌 함수
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
