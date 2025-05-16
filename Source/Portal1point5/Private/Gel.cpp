// Fill out your copyright notice in the Description page of Project Settings.


#include "Gel.h"

#include "GelPaintTarget.h"
#include "GelProjectileMovementComponent.h"
#include "Portal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGel::AGel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructionGenerateComponent();
	ConstructionInitComponent();
}

// Called when the game starts or when spawned
void AGel::BeginPlay()
{
	Super::BeginPlay();

	CustomBeginPlay();
	
}

// Called every frame
void AGel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BeforeVelocity = GelProjectileMovementComp->Velocity;
	//BeforeVelocity = SphereComp->GetPhysicsLinearVelocity();
	// if (GelState == GelStatus::Repulsion)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("CurrentVelocity => %f.%f.%f"),GelProjectileMovementComp->Velocity.X, GelProjectileMovementComp->Velocity.Y, GelProjectileMovementComp->Velocity.Z);
	// }
}

FVector AGel::GetLocation()
{
	return GetActorLocation();
}

void AGel::SetLocation(FVector newLocation)
{
	SetActorLocation(newLocation);
}

FVector AGel::GetVelocity()
{
	// if (GelState == GelStatus::Repulsion)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("GetBeforeVelocity==>>==>> %f.%f.%f"),BeforeVelocity.X, BeforeVelocity.Y, BeforeVelocity.Z);
	// }
	return BeforeVelocity;
}

void AGel::SetVelocity(FVector newVelocity)
{
	// if (GelState == GelStatus::Repulsion)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("GetVelocity==>> %f.%f.%f"),GelProjectileMovementComp->Velocity.X, GelProjectileMovementComp->Velocity.Y, GelProjectileMovementComp->Velocity.Z);
	// 	UE_LOG(LogTemp, Error, TEXT("newVelocity==>> %f.%f.%f"),newVelocity.X, newVelocity.Y, newVelocity.Z);
	// }
	GelProjectileMovementComp->Velocity = newVelocity;
	//SphereComp->SetPhysicsLinearVelocity(newVelocity);
}

FRotator AGel::GetRotation()
{
	return GetActorRotation();
}

void AGel::SetRotation(FRotator newRotation)
{
	SetActorRotation(newRotation);
}

void AGel::ConstructionGenerateComponent()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SphereComp);

	FindBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("FindBoxComp"));
	FindBoxComp->SetupAttachment(SphereComp);
	
	GelProjectileMovementComp = CreateDefaultSubobject<UGelProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	GelProjectileMovementComp->UpdatedComponent = RootComponent;
}

void AGel::ConstructionInitComponent()
{
	SphereComp->SetCollisionProfileName(TEXT("Gel"));
	SphereComp->SetNotifyRigidBodyCollision(true);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (sphereMesh.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(sphereMesh.Object);
	}
	StaticMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	FindBoxComp->SetCollisionProfileName(TEXT("NoCollision"));
	FindBoxComp->SetBoxExtent(FVector(0.f, 0.f, 0.f));
	
	GelProjectileMovementComp->Velocity = FVector::ZeroVector;


	ConstructorHelpers::FObjectFinder<UMaterialInstance> _materialInstance1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Portal1point5/Materials/Gel/MI_CleaningGel.MI_CleaningGel'"));
	if (_materialInstance1.Succeeded())
	{
		CleaningGelMaterialInstance = _materialInstance1.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> __materialInstance2(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Portal1point5/Materials/Gel/MI_ConversionGel.MI_ConversionGel'"));
	if (__materialInstance2.Succeeded())
	{
		ConversionGelMaterialInstance = __materialInstance2.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> ___materialInstance3(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Portal1point5/Materials/Gel/MI_PropulsionGel.MI_PropulsionGel'"));
	if (___materialInstance3.Succeeded())
	{
		PropulsionGelMaterialInstance = ___materialInstance3.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> ____materialInstance4(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Portal1point5/Materials/Gel/MI_RepulsionGel.MI_RepulsionGel'"));
	if (____materialInstance4.Succeeded())
	{
		RepulsionGelMaterialInstance = ____materialInstance4.Object;
	}
	
}

void AGel::CustomBeginPlay()
{
	SphereComp->OnComponentHit.AddDynamic(this, &AGel::OnComponentHit);
	SetState(GelState);
}

void AGel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//포탈은 무시
	if (nullptr != Cast<APortal>(OtherActor))
		return;
	//box위치를 충돌 지점으로 이동
	FindBoxComp->SetWorldLocation(Hit.ImpactPoint);
	//box를 터짐 영역만큼 확장
	FindBoxComp->SetBoxExtent(GelExplosionExtent);
	//box의 회전을 충돌 지점의 normal을 forward로 하도록 설정. yz basis 벡터가 중요하지 않기 때문. 평면 정보가 더 중요
	FindBoxComp->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(Hit.Normal));
	//box의 overlap효과를 켜서 감지 준비를 한다.
	FindBoxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//범위 내 actor들 찾기
	TArray<AActor*> overlappingActors;
	FindBoxComp->GetOverlappingActors(overlappingActors);
	//범위 내 대상 중에서 IGelPaintTarget 찾기
	IGelPaintTarget* target;
	for (int i = 0; i < overlappingActors.Num(); i++)
	{
		target = Cast<IGelPaintTarget>(overlappingActors[i]);
		//만약 타겟이 젤이 그려지는 대상이라면
		if (nullptr != target)
		{
			if (GelState == GelStatus::Conversion)
			{
				GelState = GelStatus::Conversion;
			}
			
			target->PaintGel(GelState);
		}
	}
	
	Destroy();
}

void AGel::SetState(GelStatus newGel)
{
	switch (newGel)
	{
		case GelStatus::Cleaning:
			StaticMeshComp->SetMaterial(0, CleaningGelMaterialInstance);
			break;
		case GelStatus::Conversion:
			StaticMeshComp->SetMaterial(0, ConversionGelMaterialInstance);
			break;
		case GelStatus::Propulsion:
			StaticMeshComp->SetMaterial(0, PropulsionGelMaterialInstance);
			break;
		case GelStatus::Repulsion:
			StaticMeshComp->SetMaterial(0, RepulsionGelMaterialInstance);
			break;
	}
	GelState = newGel;
}
