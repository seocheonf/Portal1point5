// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"

#include "PlayerCharacter.h"
#include "Portal.h"
#include "VirtualCameraForPortal.h"

// Sets default values
APortalManager::APortalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	baseComp = CreateDefaultSubobject<USceneComponent>(TEXT("baseComp"));
	SetRootComponent(baseComp);
	
}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();

	CustomBeginPlay();
	
}

// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	APlayerCharacter* a = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	OrangeVirtualCamera->UpdateVirtualCamera(a->Camera);
	BlueVirtualCamera->UpdateVirtualCamera(a->Camera);
}

void APortalManager::CustomBeginPlay()
{
	//두 포탈을 생성
	OrangePortal = GetWorld()->SpawnActor<APortal>(OriginOrangePortal);
	BluePortal = GetWorld()->SpawnActor<APortal>(OriginBluePortal);
	
	//두 포탈에 대한 가상 카메라를 생성
	OrangeVirtualCamera = GetWorld()->SpawnActor<AVirtualCameraForPortal>(AVirtualCameraForPortal::StaticClass());
	BlueVirtualCamera = GetWorld()->SpawnActor<AVirtualCameraForPortal>(AVirtualCameraForPortal::StaticClass());
	//카메라 초기화
	OrangeVirtualCamera->SetPortal(OrangePortal, BluePortal);
	BlueVirtualCamera->SetPortal(BluePortal, OrangePortal);
}