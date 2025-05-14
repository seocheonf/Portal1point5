// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomEnum.h"
#include "GelPaintTarget.h"
#include "PortableSurface.h"
#include "GameFramework/Actor.h"
#include "ActionPlane.generated.h"

UCLASS()
class PORTAL1POINT5_API AActionPlane : public AActor, public IPortableSurface, public IGelPaintTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActionPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//인터페이스 구현
public:
	virtual bool GetPortable() override;
	virtual void PaintGel(GelStatus paintingGel) override;
	
	//멤버변수
private:
	//컴파일 타임 상태 설정
	UPROPERTY(EditAnywhere)
	bool bDefaultPutPortal;
	//..?? 바로 밑의 변수 때문에 딱히 쓰지 않을 듯.
	GelStatus PlaneGel;
	//런타임 상태
	PlaneStatus PlaneState;

	UPROPERTY(EditAnywhere)
	PlaneStatus DefaultPlaneState;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* BaseComp;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* PlaneComp;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* HitComp;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OverlapComp;
	

	UPROPERTY(VisibleAnywhere) //포탈 설치 불가
	class UMaterialInstance* NoPortableMaterialInstance;
	UPROPERTY(VisibleAnywhere) //변환 (포탈 설치 가능과 공유)
	class UMaterialInstance* ConversionGelMaterialInstance;
	UPROPERTY(VisibleAnywhere) //추진
	class UMaterialInstance* PropulsionGelMaterialInstance;
	UPROPERTY(VisibleAnywhere) //반발
	class UMaterialInstance* RepulsionGelMaterialInstance;
	
	
public:

	//함수
private:
	//In Constructor Function
	//컴포넌트 생성 및 계층 설정
	void ConstructionGenerateComponent();
	//컴포넌트 정보 초기화
	void ConstructionInitComponent();

	void CustomBeginPlay();

	void SetState(PlaneStatus newPlaneState, bool bForced = false);
	void SetState(GelStatus newPlaneGel);

	void ApplyDefautState();
	void ApplyConversionState();
	void ApplyPropulsionState();
	void ApplyRepulsionState();
	
    UFUNCTION()
	void PropulsionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void PropulsionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void RepulsionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void RepulsionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	class IGelEffectTarget* ConvertActorToGelEffectTarget(AActor* actor);
	
public:

	

};
