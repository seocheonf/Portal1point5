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
	
	if (nullptr == ManagedCamera)
		return;


	
	
	
	OrangeVirtualCamera->UpdateVirtualCamera(ManagedCamera);
	BlueVirtualCamera->UpdateVirtualCamera(ManagedCamera);
}

void APortalManager::CustomBeginPlay()
{
	APlayerCharacter* ManagedPlayer = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (nullptr != ManagedPlayer)
		SetManagedCamera(ManagedPlayer->PlayerCameraComp);
	else
		UE_LOG(LogTemp, Error, TEXT("Not Player"));
	
	//두 포탈을 생성
	OrangePortal = GetWorld()->SpawnActor<APortal>(OriginOrangePortal);
	BluePortal = GetWorld()->SpawnActor<APortal>(OriginBluePortal);
	
	//두 포탈에 대한 가상 카메라를 생성
	OrangeVirtualCamera = GetWorld()->SpawnActor<AVirtualCameraForPortal>(AVirtualCameraForPortal::StaticClass());
	BlueVirtualCamera = GetWorld()->SpawnActor<AVirtualCameraForPortal>(AVirtualCameraForPortal::StaticClass());
	//카메라 초기화
	int x = 0;
	int y = 0;
	GetWorld()->GetFirstPlayerController()->GetViewportSize(x, y);
	OrangeVirtualCamera->SetVirtualCameraForPortal(OrangePortal, BluePortal, x, y);
	
	OrangeVirtualCamera->SetPortal(OrangePortal, BluePortal);
	BlueVirtualCamera->SetPortal(BluePortal, OrangePortal);
}

void APortalManager::SetManagedCamera(class UCameraComponent* managedCamera)
{
	ManagedCamera = managedCamera;
}
