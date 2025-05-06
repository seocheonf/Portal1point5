// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "CustomUnrealExtention.h"
#include "MovableTarget.h"
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

	PortalMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalComp"));
	PortalMeshComp->SetupAttachment(BaseComp);
}

void APortal::ConstructionInitComponent()
{
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> temptPortalMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (temptPortalMesh.Succeeded())
	{
		PortalMeshComp->SetStaticMesh(temptPortalMesh.Object);
	}
	
	//blockall - 추후 통과 가능 시 overlap쪽으로 수정해야 함.
	PortalMeshComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	//PortalMeshComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//충돌 반응 On
	PortalMeshComp->SetNotifyRigidBodyCollision(true);
	
	PortalMeshComp->SetMaterial(0, nullptr);
}

void APortal::CustomBeginPlay()
{
	//PortalMeshComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnComponentBeginOverlap);
	PortalMeshComp->OnComponentHit.AddDynamic(this, &APortal::OnComponentHit);
}

void APortal::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

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
	
	FVector newLocation = CustomUnrealExtention::TransformRelativeLocation(LinkedPortal->GetActorTransform(), baseTransform, OtherActor->GetActorLocation());
	OtherActor->SetActorLocation(newLocation + LinkedPortal->GetActorForwardVector() * 100.f);
	
	FRotator newRotation = CustomUnrealExtention::TransformRelativeRotation(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetRotation());
	movableTarget->SetRotation(newRotation);

	FVector newDirection = CustomUnrealExtention::TransformRelativeDirection(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetVelocity());
	movableTarget->SetVelocity(newDirection);
	
}

void APortal::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	UE_LOG(LogTemp, Error, TEXT("Test hit"));

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
	
	FVector newLocation = CustomUnrealExtention::TransformRelativeLocation(LinkedPortal->GetActorTransform(), baseTransform, OtherActor->GetActorLocation());
	OtherActor->SetActorLocation(newLocation + LinkedPortal->GetActorForwardVector() * 100.f);
	
	FRotator newRotation = CustomUnrealExtention::TransformRelativeRotation(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetRotation());
	movableTarget->SetRotation(newRotation);

	FVector newDirection = CustomUnrealExtention::TransformRelativeDirection(LinkedPortal->GetActorTransform(), baseTransform, movableTarget->GetVelocity());
	movableTarget->SetVelocity(newDirection);
}

void APortal::SetPortal(UTextureRenderTarget2D* renderTarget, APortal* linkedPortal)
{

	PortalMaterial = PortalMeshComp->CreateDynamicMaterialInstance(0, OriginPortalMaterial);
	PortalMaterial->SetTextureParameterValue(FName("RenderTarget"), renderTarget);

	LinkedPortal = linkedPortal;
}

