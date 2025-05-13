// Fill out your copyright notice in the Description page of Project Settings.


#include "Gel.h"

#include "GelPaintTarget.h"
#include "GelProjectileMovementComponent.h"
#include "Portal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	//UE_LOG(LogTemp, Error, TEXT("%f.%f.%f"),ProjectileMovementComp->Velocity.X, ProjectileMovementComp->Velocity.Y, ProjectileMovementComp->Velocity.Z);
	
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
	return BeforeVelocity;
}

void AGel::SetVelocity(FVector newVelocity)
{
	//UE_LOG(LogTemp, Error, TEXT("%f.%f.%f"),ProjectileMovementComp->Velocity.X, ProjectileMovementComp->Velocity.Y, ProjectileMovementComp->Velocity.Z);
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

	GelProjectileMovementComp->Velocity = FVector::ZeroVector;
}

void AGel::CustomBeginPlay()
{
	SphereComp->OnComponentHit.AddDynamic(this, &AGel::OnComponentHit);
}

void AGel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (nullptr != Cast<APortal>(OtherActor))
		return;

	IGelPaintTarget* target = Cast<IGelPaintTarget>(OtherActor);

	//만약 타겟이 젤이 그려지는 대상이라면
	if (nullptr != target)
	{
		target->PaintGel()
	
	}
	
	Destroy();
}
