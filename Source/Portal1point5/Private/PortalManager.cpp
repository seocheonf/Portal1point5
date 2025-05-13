// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"

#include "CustomUnrealExtention.h"
#include "MathUtil.h"
#include "MyActor.h"
#include "PlayerCharacter.h"
#include "Portal.h"
#include "VirtualCameraForPortal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APortalManager::APortalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	baseComp = CreateDefaultSubobject<USceneComponent>(TEXT("baseComp"));
	SetRootComponent(baseComp);


	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> b(TEXT("/Script/Engine.TextureRenderTarget2D'/Game/Portal1point5/Textures/RT_BluePortal.RT_BluePortal'"));
	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> o(TEXT("/Script/Engine.TextureRenderTarget2D'/Game/Portal1point5/Textures/RT_OrangePortal.RT_OrangePortal'"));

	if (b.Succeeded() && o.Succeeded())
	{
		BlueRenderTarget = b.Object;
		OrangeRenderTarget = o.Object;
	}
	else
	{
		
	}
	
	
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
	{
		SetManagedCamera(ManagedPlayer->PlayerCameraComp);
		ManagedPlayer->SetPortalManager(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Player"));
	}

	//두 포탈을 생성
	OrangePortal = GetWorld()->SpawnActor<APortal>(OriginOrangePortal);
	BluePortal = GetWorld()->SpawnActor<APortal>(OriginBluePortal);

	//두 포탈에 대한 가상 카메라를 생성
	OrangeVirtualCamera = GetWorld()->SpawnActor<AVirtualCameraForPortal>(AVirtualCameraForPortal::StaticClass());
	BlueVirtualCamera = GetWorld()->SpawnActor<AVirtualCameraForPortal>(AVirtualCameraForPortal::StaticClass());

	//포탈용 렌더타겟 생성
	//OrangeRenderTarget = NewObject<UTextureRenderTarget2D>();
	//BlueRenderTarget = NewObject<UTextureRenderTarget2D>();
	//GenerateRenderTarget();
	
	SetPortal();
	
	SetVirtualCamera();

	// if (!BlueRenderTarget)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("RenderTarget is nullptr"));
	// }
	// else if (!BlueRenderTarget->GetResource())
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("RenderTarget->Resource is nullptr"));
	// }
	// else if (!BlueRenderTarget->GetResource->())
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("RenderTarget->TextureRHI is not valid"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("RenderTarget is valid and ready!"));
	// }
}

void APortalManager::PutPortal(APortal* targetPortal, const AActor* instigator, const FVector& startPoint, const FVector& direction)
{
	FHitResult hit;
	FVector endPoint = startPoint + direction * 10000.f;
	FCollisionQueryParams traceColParam = FCollisionQueryParams::DefaultQueryParam;
	traceColParam.AddIgnoredActor(instigator);
	//ECC_GameTraceChannel1 => PortalTrace TraceChannel
	GetWorld()->LineTraceSingleByChannel(hit, startPoint, endPoint, ECollisionChannel::ECC_GameTraceChannel1, traceColParam);

	IPortableSurface* hitSurface = Cast<IPortableSurface>(hit.GetActor());
	if ((nullptr == hitSurface) || (!hitSurface->GetPortable()))
	{
		//미감지
		return;
	}

	FVector targetForwardVector = hit.ImpactNormal;
	FVector targetUpVector;
	//XY 평면 확인
	if (FMathf::Abs(hit.ImpactNormal.X) < 0.0001f && FMathf::Abs(hit.ImpactNormal.Y) < 0.0001f)
	{
		targetUpVector = CustomUnrealExtention::OrthographicProjection(direction, hit.ImpactNormal);
	}
	else
	{
		targetUpVector = CustomUnrealExtention::OrthographicProjection(FVector::UpVector, hit.ImpactNormal);
	}

	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(targetForwardVector, targetUpVector);

	//z fighting problem temporary solving
	targetPortal->SetActorLocation(hit.ImpactPoint + hit.ImpactNormal*0.1f);
	targetPortal->SetActorRotation(rot);
}
//gpt 복붙임. 테스트 용으로 썼던 것.
void APortalManager::GenerateRenderTarget()
{
	BlueRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	BlueRenderTarget->InitAutoFormat(1024, 1024);
	BlueRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
	BlueRenderTarget->ClearColor = FLinearColor::Black;
	BlueRenderTarget->UpdateResourceImmediate(true);
	BlueRenderTarget->AddToRoot();

	OrangeRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	OrangeRenderTarget->InitAutoFormat(1024, 1024);
	OrangeRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
	OrangeRenderTarget->ClearColor = FLinearColor::Black;
	OrangeRenderTarget->UpdateResourceImmediate(true);
	OrangeRenderTarget->AddToRoot();
}

void APortalManager::SetManagedCamera(class UCameraComponent* managedCamera)
{
	ManagedCamera = managedCamera;
}

void APortalManager::SetPortal()
{
	OrangePortal->SetPortal(OrangeRenderTarget, BluePortal);
	BluePortal->SetPortal(BlueRenderTarget, OrangePortal);
}

void APortalManager::SetVirtualCamera()
{
	//카메라 초기화
	int resolutionX = 0;
	int resolutionY = 0;
	
	//GetWorld()->GetFirstPlayerController()->GetViewportSize(viewPortX, viewPortY);
	FIntPoint resolution = UGameUserSettings::GetGameUserSettings()->GetScreenResolution();
	resolutionX = resolution.X;
	resolutionY = resolution.Y;
	
	OrangeVirtualCamera->SetVirtualCameraForPortal(OrangeRenderTarget, OrangePortal, BluePortal, resolutionX, resolutionY);
	BlueVirtualCamera->SetVirtualCameraForPortal(BlueRenderTarget, BluePortal, OrangePortal, resolutionX, resolutionY);
}

void APortalManager::PutOrangePortal(const AActor* instigator, const FVector& startPoint, const FVector& direction)
{
	PutPortal(OrangePortal, instigator, startPoint, direction);
}

void APortalManager::PutBluePortal(const AActor* instigator, const FVector& startPoint, const FVector& direction)
{
	PutPortal(BluePortal, instigator, startPoint, direction);
}




