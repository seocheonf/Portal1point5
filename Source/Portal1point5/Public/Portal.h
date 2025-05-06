// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class PORTAL1POINT5_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	//멤버변수
private:

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* BaseComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PortalMeshComp;

	class UMaterialInstanceDynamic* PortalMaterial;

	class APortal* LinkedPortal;
	
public:
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* OriginPortalMaterial;


	//함수
private:

	//In Constructor Function
	//컴포넌트 생성 및 계층 설정
	void ConstructionGenerateComponent();
	//컴포넌트 정보 초기화
	void ConstructionInitComponent();

	//커스텀
	void CustomBeginPlay();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:

	void SetPortal(UTextureRenderTarget2D* renderTarget, APortal* linkedPortal);
	
};
