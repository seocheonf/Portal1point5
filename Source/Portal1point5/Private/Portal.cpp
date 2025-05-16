// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "CustomUnrealExtention.h"
#include "MovableTarget.h"
#include "Components/BoxComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ConstructionGenerateComponent();
	ConstructionInitComponent();


	
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	CustomBeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//==커스텀===

void APortal::ConstructionGenerateComponent()
{
	BaseComp = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComp"));
	SetRootComponent(BaseComp);

	PortalBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalBoxComp"));
	PortalBoxComp->SetupAttachment(BaseComp);
		
	PortalMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalComp"));
	//PortalMeshComp->SetupAttachment(BaseComp);
	PortalMeshComp->SetupAttachment(PortalBoxComp);

	PortalBorderMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalBorderComp"));
	PortalBorderMeshComp->SetupAttachment(PortalBoxComp);
	
}

void APortal::ConstructionInitComponent()
{
	//mesh의 충돌 처리 옮겨두기
	PortalBoxComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	//충돌 반응 On
	PortalBoxComp->SetNotifyRigidBodyCollision(true);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> temptPortalMesh(TEXT("/Script/Engine.StaticMesh'/Game/Portal1point5/Meshes/SM_Portal.SM_Portal'"));
	if (temptPortalMesh.Succeeded())
	{
		PortalMeshComp->SetStaticMesh(temptPortalMesh.Object);
	}
	
	//blockall - 추후 통과 가능 시 overlap쪽으로 수정해야 함.
	//PortalMeshComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	//PortalMeshComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//==PortalBoxComp로 옮겼기에 충돌 관련 끄기
	PortalMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	//충돌 반응 On
	//PortalMeshComp->SetNotifyRigidBodyCollision(true);
	
	PortalMeshComp->SetMaterial(0, nullptr);

	//==
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> temptPortalBorderMesh(TEXT("/Script/Engine.StaticMesh'/Game/Portal1point5/Meshes/SM_PortalBorder.SM_PortalBorder'"));

	if (temptPortalBorderMesh.Succeeded())
	{
		PortalBorderMeshComp->SetStaticMesh(temptPortalBorderMesh.Object);
	}

	PortalBorderMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	
	PortalBorderMeshComp->SetMaterial(0, nullptr);
	
}

void APortal::CustomBeginPlay()
{
	//PortalMeshComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnComponentBeginOverlap);
	//PortalMeshComp->OnComponentHit.AddDynamic(this, &APortal::OnComponentHit);

	//PortalBoxComp로 옮겼기에 충돌 함수 등록도 수정
	PortalBoxComp->OnComponentHit.AddDynamic(this, &APortal::OnComponentHit);
}

void APortal::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GFrameNumber == BeforeHitFrameNumber && OtherActor == BeforeHitActor)
	{
		return;
	}
	BeforeHitFrameNumber = GFrameNumber;
	BeforeHitActor = OtherActor;

	UE_LOG(LogTemp, Error, TEXT("Test overlap"));

	IMovableTarget* movableTarget = Cast<IMovableTarget>(OtherActor);
	if (nullptr == movableTarget)
		return;

	UE_LOG(LogTemp, Error, TEXT("Test overlap2"));

	//==연결된 포탈을 가정하는 과정==//
	//현재 포탈의 z축 기준으로 180도 뒤집어진 방향을 나타내는 Rotation.
	//이를 통해 연결된 포탈의 Rotation을 가정
	FRotator baseRotation = UKismetMathLibrary::MakeRotationFromAxes(GetActorForwardVector() * -1, GetActorRightVector() * -1, GetActorUpVector());
	//연결된 포탈의 Transform을 가정
	FTransform baseTransform(baseRotation, GetActorLocation(), GetActorScale());
	
	// FVector newLocation = CustomUnrealExtention::TransformRelativeLocation(LinkedPortal->GetActorTransform(), baseTransform, OtherActor->GetActorLocation());
	// FRotator newRotation = CustomUnrealExtention::TransformRelativeRotation(LinkedPortal->GetActorTransform(), baseTransform, OtherActor->GetActorRotation());
	//
	// OtherActor->SetActorLocation(newLocation + LinkedPortal->GetActorForwardVector() * 100.f);
	// OtherActor->SetActorRotation(newRotation);
	//
	// FVector newDirection = CustomUnrealExtention::TransformRelativeDirection(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetVelocity());
	// movableTarget->SetVelocity(newDirection);
	
	//FVector newLocation = CustomUnrealExtention::TransformRelativeLocation(LinkedPortal->GetActorTransform(), GetActorTransform(), movableTarget->GetLocation());
	//movableTarget->SetLocation(newLocation + LinkedPortal->GetActorForwardVector() * 100.f);
	//좌우로는 대칭이동해야 해서, 가상 포탈로 가정하여 상대위치 구한 뒤, linked portal의 전방으로 이동하는 식으로 수정함.
	//forward vector에 평면과의 거리의 2배로 계산하면 됨.
	FVector newLocation = CustomUnrealExtention::TransformRelativeLocation(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetLocation());
	float distance = FVector::DotProduct(GetActorForwardVector(), movableTarget->GetLocation() - GetActorLocation());
	newLocation += LinkedPortal->GetActorForwardVector() * distance * 2;
	movableTarget->SetLocation(newLocation);
	
	FRotator newRotation = CustomUnrealExtention::TransformRelativeRotation(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetRotation());
	movableTarget->SetRotation(newRotation);

	FVector newDirection = CustomUnrealExtention::TransformRelativeDirection(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetVelocity());
	movableTarget->SetVelocity(newDirection);

	UE_LOG(LogTemp, Error, TEXT("Test overlap End"));
	
}

void APortal::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (GFrameNumber == BeforeHitFrameNumber && OtherActor == BeforeHitActor)
	{
		return;
	}
	BeforeHitFrameNumber = GFrameNumber;
	BeforeHitActor = OtherActor;
	
	UE_LOG(LogTemp, Error, TEXT("Test hit"));
	UE_LOG(LogTemp, Error, TEXT("%s"), *OtherActor->GetName());
	IMovableTarget* movableTarget = Cast<IMovableTarget>(OtherActor);
	if (nullptr == movableTarget)
		return;

	UE_LOG(LogTemp, Error, TEXT("Test hit2"));

	//==연결된 포탈을 가정하는 과정==//
	//현재 포탈의 z축 기준으로 180도 뒤집어진 방향을 나타내는 Rotation.
	//이를 통해 연결된 포탈의 Rotation을 가정
	FRotator baseRotation = UKismetMathLibrary::MakeRotationFromAxes(GetActorForwardVector() * -1, GetActorRightVector() * -1, GetActorUpVector());
	//연결된 포탈의 Transform을 가정
	FTransform baseTransform(baseRotation, GetActorLocation(), GetActorScale());
	
	//FVector newLocation = CustomUnrealExtention::TransformRelativeLocation(LinkedPortal->GetActorTransform(), GetActorTransform(), movableTarget->GetLocation());
	//movableTarget->SetLocation(newLocation + LinkedPortal->GetActorForwardVector() * 100.f);
	//좌우로는 대칭이동해야 해서, 가상 포탈로 가정하여 상대위치 구한 뒤, linked portal의 전방으로 이동하는 식으로 수정함.
	//forward vector에 평면과의 거리의 2배로 계산하면 됨.
	
	//근데 포탈은 통과를 가정하고 있어서, 지금과 같이 충돌 즉시 날려보내는 거는 고려되지 않음.
	//그래서 벽에 박히니, 임시로 적당히 이동시키기. 95cm정도. 이것도 불안정함. (캡슐 절반 높이, 반지름 활용해서 대각선 값 넣은 거)
	
	// FVector newLocation = CustomUnrealExtention::TransformRelativeLocation(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetLocation());
	// float distance = FVector::DotProduct(GetActorForwardVector(), movableTarget->GetLocation() - GetActorLocation());
	// newLocation += LinkedPortal->GetActorForwardVector() * distance * 2;
	FVector newLocation = LinkedPortal->GetActorLocation() + LinkedPortal->GetActorForwardVector() * 95.f;
	movableTarget->SetLocation(newLocation);
	
	FRotator newRotation = CustomUnrealExtention::TransformRelativeRotation(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetRotation());
	movableTarget->SetRotation(newRotation);

	FVector newDirection = CustomUnrealExtention::TransformRelativeDirection(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetVelocity());
	if (newDirection.Length() < 500.f)
	{
		newDirection.Normalize();
		newDirection *= 500.f;
	}
	//임시 탈출 보정 속도 (비스듬한 접근에서)
	if (FMath::Abs(FVector::DotProduct(newDirection.GetSafeNormal(), GetActorForwardVector())) < 0.5f)
	{
		newDirection += LinkedPortal->GetActorForwardVector() * 500.f;
	}
	movableTarget->SetVelocity(newDirection);
	
	UE_LOG(LogTemp, Error, TEXT("Test hit End"));

}

void APortal::SetPortal(UTextureRenderTarget2D* renderTarget, APortal* linkedPortal, UMaterial* borderMaterial)
{

	PortalMaterial = PortalMeshComp->CreateDynamicMaterialInstance(0, OriginPortalMaterial);
	PortalMaterial->SetTextureParameterValue(FName("RenderTarget"), renderTarget);

	PortalBorderMeshComp->SetMaterial(0, borderMaterial);
	
	LinkedPortal = linkedPortal;
}

