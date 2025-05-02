// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

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
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//==커스텀===

void APortal::ConstructionGenerateComponent()
{
	BaseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseComp"));
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
	PortalMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	
	
}

