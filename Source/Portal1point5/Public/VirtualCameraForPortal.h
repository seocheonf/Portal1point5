// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VirtualCameraForPortal.generated.h"

UCLASS()
class PORTAL1POINT5_API AVirtualCameraForPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVirtualCameraForPortal();

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
	class USceneCaptureComponent2D* SceneCaptureComp;

	class APortal* RenderingPortal;
	class APortal* LinkedPortal;

	class UTextureRenderTarget2D* RenderTarget;
	
	//함수
private:

	//In Constructor Function
	//컴포넌트 생성 및 계층 설정
	void ConstructionGenerateComponent();
	//컴포넌트 정보 초기화
	void ConstructionInitComponent();

	//CustomBeginPlay
	void CustomBeginPlay();
	
	//본인 위치와 회전을 플레이어 카메라, 관리 포탈 등을 기반으로 갱신
	//매개변수로 주어진 카메라와 가상 카메라의 관리 포탈 사이의 상대 좌표 계산 및 가상 카메라 이동
	void TransformFromTargetCamera(class UCameraComponent* targetCamera);

	//Clip Plane 설정
	void SetClipPlane(FVector normal, FVector location);

	//카메라 동기화
	void Synchronization(class UCameraComponent* targetCamera);

public:

	//관리 포탈 설정 
	void SetPortal(class APortal* renderingPortal, class APortal* linkedPortal);

	//렌더 타겟 사이즈 조정
	void SetRenderTargetSize(int x, int y);

	//커스텀 초기화
	void SetVirtualCameraForPortal(APortal* renderingPortal, APortal* linkedPortal, int renderTargetSizeX, int renderTargetSizeY);
	
	//타겟 카메라 기반, 가상 카메라 업데이트
	void UpdateVirtualCamera(class UCameraComponent* targetCamera);
	
};
