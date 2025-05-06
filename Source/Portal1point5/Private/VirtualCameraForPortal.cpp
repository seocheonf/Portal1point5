// Fill out your copyright notice in the Description page of Project Settings.


#include "VirtualCameraForPortal.h"

#include "CustomUnrealExtention.h"
#include "Portal.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVirtualCameraForPortal::AVirtualCameraForPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ConstructionGenerateComponent();
	ConstructionInitComponent();
	
}

// Called when the game starts or when spawned
void AVirtualCameraForPortal::BeginPlay()
{
	Super::BeginPlay();
	
	CustomBeginPlay();
}

// Called every frame
void AVirtualCameraForPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//==커스텀===

void AVirtualCameraForPortal::ConstructionGenerateComponent()
{
	BaseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseComp"));
	SetRootComponent(BaseComp);

	//Root의 Transform과는 별개로 Child의 Transform이 변경될 상황을 대비하기 위해 SceneCaptureComponent를 SceneComponent의 하위로 감쌌음.
	SceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
	SceneCaptureComp->SetupAttachment(BaseComp);
	SceneCaptureComp->bEnableClipPlane = true;
	
}

void AVirtualCameraForPortal::ConstructionInitComponent()
{
}

void AVirtualCameraForPortal::CustomBeginPlay()
{
	
}

void AVirtualCameraForPortal::TransformFromTargetCamera(UCameraComponent* targetCameraComp)
{

	//카메라의 world상 좌표와 회전이 중요. 내가 화면을 보고 있는 실질적 환경을 기준으로 가상 카메라의 Trasnform이 결정되어야 하므로.
	
	//연결된 포탈을 기반으로 하는 상대적 Transform을 잡아 실제 그 정보대로 이동
	
	//일단 현재 플레이어 카메라 매니저를 가져와 두자.
	//APlayerCameraManager* playerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	//==연결된 포탈을 가정하는 과정==//
	
	FVector RenderingPortalForward = RenderingPortal->GetActorForwardVector();
	FVector RenderingPortalRight = RenderingPortal->GetActorRightVector();
	FVector RenderingPortalUp = RenderingPortal->GetActorUpVector();
	
	//렌더링 포탈의 z축 기준으로 180도 뒤집어진 방향을 나타내는 Rotation.
	//이를 통해 연결된 포탈의 Rotation을 가정
	FRotator baseRotation = UKismetMathLibrary::MakeRotationFromAxes(RenderingPortalForward * -1, RenderingPortalRight * -1, RenderingPortalUp);
	//연결된 포탈의 Transform을 가정
	FTransform baseTransform(baseRotation, RenderingPortal->GetActorLocation(), RenderingPortal->GetActorScale());

	
	//==== 가정 포탈을 기반으로 상대좌표를 구하고 연결된 포탈 기반의 상대좌표로 이동 ====//
	//==== 가정 포탈을 기반으로 상대방향을 구하고 연결된 포탈 기반의 상대방향으로 회전 ====//
	
	//가정된 Transform을 기반으로 현재 플레이어 카메라의 local location를 구하고
	//이 local location을 실제 대상의 Transform을 기반으로 적용하여 world location 결정 
	FVector realLocation = CustomUnrealExtention::TransformRelativeLocation(LinkedPortal->GetActorTransform(), baseTransform, targetCameraComp->GetComponentLocation());
	//이 world location을 본인의 위치로 설정. (이 이동은 Root가 해야 함)
	SetActorLocation(realLocation);
	
	//현재 플레이어 카메라의 전방(x축) Vector를 구한다.
	//가정된 Transform을 기반으로 하는 현재 플레이어 카메라의 전방(x축)의 local direction을 구한다.
	//현재 플레이어 카메라의 상향(z축) Vector를 구한다. (추후 회전 설정 시 정확한 방향을 위해 두개의 좌표가 필요하기 때문)
	//가정된 Transform을 기반으로 하는 현재 플레이어 카메라의 상향(z축)의 local direction을 구한다.
	//이 local direction을 실제 대상의 Transform을 기반으로 적용하여 world rotation 결정
	//이 local Forward와 Up에 대해서 각각, 대상의 Transform 기반 Direction을 계산
	//이 Forward와 Up Direction을 기반으로 회전 계산 (두 축을 바탕으로 (이 기능의 x는 전방, z는 상향이므로 최초 forward와 up을 전방, 상향에서 가져온 것을 대입하면 됨) 정확한 방향 설정)
	FRotator realRotation = CustomUnrealExtention::TransformRelativeRotation(LinkedPortal->GetActorTransform(), baseTransform, targetCameraComp->GetComponentRotation());
	
	//이 world rotation을 본인의 방향으로 설정. (이 회전은 Root가 해야 함)
	SetActorRotation(realRotation);
	
}

void AVirtualCameraForPortal::SetClipPlane(FVector normal, FVector location)
{
	SceneCaptureComp->ClipPlaneBase = location;
	SceneCaptureComp->ClipPlaneNormal = normal;
}

void AVirtualCameraForPortal::Synchronization(class UCameraComponent* targetCamera)
{
	SceneCaptureComp->FOVAngle = targetCamera->FieldOfView;
}

void AVirtualCameraForPortal::SetPortal(APortal* renderingPortal, APortal* linkedPortal)
{
	RenderingPortal = renderingPortal;
	LinkedPortal = linkedPortal;
}

void AVirtualCameraForPortal::SetRenderTargetSize(int x, int y)
{
	RenderTarget->SizeX = x;
	RenderTarget->SizeY = y;
}

void AVirtualCameraForPortal::SetVirtualCameraForPortal(UTextureRenderTarget2D* renderTarget, APortal* renderingPortal, APortal* linkedPortal, int renderTargetSizeX, int renderTargetSizeY)
{
	SetPortal(renderingPortal, linkedPortal);
	SceneCaptureComp->TextureTarget = renderTarget;
	RenderTarget = renderTarget;
	SetRenderTargetSize(renderTargetSizeX, renderTargetSizeY);
}

void AVirtualCameraForPortal::UpdateVirtualCamera(UCameraComponent* targetCameraComp)
{
	Synchronization(targetCameraComp);
	TransformFromTargetCamera(targetCameraComp);
	SetClipPlane(LinkedPortal->GetActorForwardVector(), LinkedPortal->GetActorLocation());
}
