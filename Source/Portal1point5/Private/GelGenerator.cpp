// Fill out your copyright notice in the Description page of Project Settings.


#include "GelGenerator.h"

#include "Gel.h"
#include "Components/ArrowComponent.h"

// Sets default values
AGelGenerator::AGelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructionGenerateComponent();
	ConstructionInitComponent();
	
}

// Called when the game starts or when spawned
void AGelGenerator::BeginPlay()
{
	Super::BeginPlay();
	CustomBeginPlay();	
}

// Called every frame
void AGelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGelGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (TimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

void AGelGenerator::ConstructionGenerateComponent()
{
	BaseComp = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComp"));
	SetRootComponent(BaseComp);
	
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(BaseComp);
}

void AGelGenerator::ConstructionInitComponent()
{
	ArrowComp->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}
void AGelGenerator::CustomBeginPlay()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGelGenerator::GenerateGel, ShootInterval, true);
}

void AGelGenerator::GenerateGel()
{
	float offsetRandom = FMath::RandRange(-25.f, 25.f);
	FVector offsetLocation = ArrowComp->GetComponentLocation();
	offsetLocation += FVector(offsetRandom, offsetRandom, offsetRandom);
	
	float sizeRandom = FMath::RandRange(0.3f, 0.6f);
	
	GeneratedGel = GetWorld()->SpawnActor<AGel>(OriginGel, offsetLocation, ArrowComp->GetComponentRotation());
	GeneratedGel->SetState(GelState);
	GeneratedGel->SetVelocity(ArrowComp->GetForwardVector() * ShootSpeed);
	GeneratedGel->SetActorScale3D(FVector(sizeRandom, sizeRandom, sizeRandom));
}

