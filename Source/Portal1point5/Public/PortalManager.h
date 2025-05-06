// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalManager.generated.h"

UCLASS()
class PORTAL1POINT5_API APortalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//멤버변수
private:

	class USceneComponent* baseComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APortal> OriginOrangePortal;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APortal> OriginBluePortal;
	class APortal* OrangePortal;
	class APortal* BluePortal;

	class AVirtualCameraForPortal* OrangeVirtualCamera;
	class AVirtualCameraForPortal* BlueVirtualCamera;

	class UCameraComponent* ManagedCamera;

	UPROPERTY(VisibleAnywhere)
	class UTextureRenderTarget2D* OrangeRenderTarget;
	UPROPERTY(VisibleAnywhere)
	class UTextureRenderTarget2D* BlueRenderTarget;
	
	
	//함수
private:
	//CustomBeginPlay
	void CustomBeginPlay();
	void PutPortal(APortal* targetPortal, const AActor* instigator, const FVector& startPoint, const FVector& direction);

public:
	void SetManagedCamera(class UCameraComponent* managedCamera);

	void SetPortal();
	void SetVirtualCamera();

	void PutOrangePortal(const AActor* instigator, const FVector& startPoint, const FVector& direction);
	void PutBluePortal(const AActor* instigator, const FVector& startPoint, const FVector& direction);
};
