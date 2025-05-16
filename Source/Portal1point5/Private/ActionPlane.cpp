// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlane.h"

#include "CustomEnum.h"
#include "GelEffectTarget.h"
#include "InGameValue.h"
#include "Components/BoxComponent.h"

// Sets default values
AActionPlane::AActionPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructionGenerateComponent();
	ConstructionInitComponent();
}

// Called when the game starts or when spawned
void AActionPlane::BeginPlay()
{
	Super::BeginPlay();

	CustomBeginPlay();

}

// Called every frame
void AActionPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AActionPlane::GetPortable()
{
	return (bDefaultPutPortal || GelStatus::Conversion == PlaneGel);
}

void AActionPlane::PaintGel(GelStatus paintingGel)
{
	SetState(paintingGel);
}

void AActionPlane::ConstructionGenerateComponent()
{
	BaseComp = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComp"));
	SetRootComponent(BaseComp);
	
	PlaneComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComp"));
	PlaneComp->SetupAttachment(BaseComp);

	HitComp = CreateDefaultSubobject<UBoxComponent>(TEXT("HitComp"));
	HitComp->SetupAttachment(BaseComp);

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetupAttachment(BaseComp);
	
}

void AActionPlane::ConstructionInitComponent()
{
	PlaneComp->SetRelativeRotation(FRotator(-90.f, 0, 0));
	PlaneComp->SetWorldScale3D(FVector(0.5f, 0.5f, 1.f));
	PlaneComp->SetCollisionProfileName(TEXT("NoCollision"));

	HitComp->SetBoxExtent(FVector(2.5f, 25.f, 25.f));
	HitComp->SetRelativeLocation(FVector(-2.5f, 0.f, 0.f));
	HitComp->SetCollisionProfileName(TEXT("PortalHit"));

	OverlapComp->SetBoxExtent(FVector(5.f, 25.f, 25.f));
	OverlapComp->SetCollisionProfileName(TEXT("PortalOverlap"));
	
	ConstructorHelpers::FObjectFinder<UMaterialInstance> _materialInstance1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Portal1point5/Materials/Plane/MI_NoPortablePlane.MI_NoPortablePlane'"));
	if (_materialInstance1.Succeeded())
	{
		NoPortableMaterialInstance = _materialInstance1.Object;
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

void AActionPlane::CustomBeginPlay()
{
	SetState(DefaultPlaneState, true);
}

void AActionPlane::SetState(PlaneStatus newPlaneState, bool bForced)
{
	if (!bForced && PlaneState == newPlaneState)
		return;
	
	//HitComp->OnComponentHit.RemoveAll(this);
	OverlapComp->OnComponentBeginOverlap.RemoveAll(this);
	OverlapComp->OnComponentEndOverlap.RemoveAll(this);
	switch (newPlaneState)
	{
		case PlaneStatus::Default:
			ApplyDefautState();
			break;
		case PlaneStatus::Conversion:
			ApplyConversionState();
			break;
		case PlaneStatus::Repulsion:
			ApplyRepulsionState();
			break;
		case PlaneStatus::Propulsion:
			ApplyPropulsionState();
			break;
	}
	
	PlaneState = newPlaneState;
}

void AActionPlane::SetState(GelStatus newPlaneGel)
{
	if (PlaneGel == newPlaneGel)
		return;
	
	switch (newPlaneGel)
	{
		case GelStatus::Cleaning:
			SetState(PlaneStatus::Default);
			break;
		case GelStatus::Conversion:
			SetState(PlaneStatus::Conversion);
			break;
		case GelStatus::Repulsion:
			SetState(PlaneStatus::Repulsion);
			break;
		case GelStatus::Propulsion:
			SetState(PlaneStatus::Propulsion);
			break;
	}

	PlaneGel = newPlaneGel;
}

void AActionPlane::ApplyDefautState()
{
	if (bDefaultPutPortal)
	{
		PlaneComp->SetMaterial(0, ConversionGelMaterialInstance);
	}
	else
	{
		PlaneComp->SetMaterial(0, NoPortableMaterialInstance);
	}
}

void AActionPlane::ApplyConversionState()
{
	PlaneComp->SetMaterial(0, ConversionGelMaterialInstance);
	
}

void AActionPlane::ApplyPropulsionState()
{
	PlaneComp->SetMaterial(0, PropulsionGelMaterialInstance);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AActionPlane::PropulsionBeginOverlap);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &AActionPlane::PropulsionEndOverlap);
}

void AActionPlane::ApplyRepulsionState()
{
	PlaneComp->SetMaterial(0, RepulsionGelMaterialInstance);
	
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AActionPlane::RepulsionBeginOverlap);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &AActionPlane::RepulsionEndOverlap);
}

void AActionPlane::PropulsionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IGelEffectTarget* target = ConvertActorToGelEffectTarget(OtherActor);
	if (nullptr == target)
		return;

	target->IncrementEffectCount(GelEffectType::SpeedUp);
	int effectCount = target->GetEffectCount(GelEffectType::SpeedUp);
	if (effectCount > 1)
	{
		return;
	}
	if (effectCount < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Error!!!!! Invalid Count!"))
		return;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Propulsion"));
	UE_LOG(LogTemp, Error, TEXT("%s"), *OtherActor->GetName());
	
	//만약, 평면의 각도가 캐릭터가 밟을 수 있는 각도를 넘어서게 된다면, 반응하지 않음.
	float dot = GetActorForwardVector().Dot(FVector::UpVector);
	float degree = FMath::RadiansToDegrees(FMath::Acos(dot));
	if (degree > 90 - WALKABLE_SLOPE_DEGREE)
	{
		return;
	}
	
	GelEffectInfo targetInfo = GelEffectInfo();
	FVector empty = FVector::ZeroVector;
	target->GetMovementInfo(targetInfo, empty);

	targetInfo.targetAcceleration *= 2.5f;
	targetInfo.targetMoveSpeed *= 2.5f;
	targetInfo.targetFriction /= 2.5f;

	target->SetMovementInfo(targetInfo);
}

void AActionPlane::PropulsionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//원래대로 값을 복구
	IGelEffectTarget* target = ConvertActorToGelEffectTarget(OtherActor);
	if (nullptr == target)
		return;

	target->DecrementEffectCount(GelEffectType::SpeedUp);
	int effectCount = target->GetEffectCount(GelEffectType::SpeedUp);
	if (effectCount > 0)
	{
		return;
	}
	if (effectCount < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Error!!!!! Invalid Count!"))
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("PropulsionEnd"));
	UE_LOG(LogTemp, Error, TEXT("%s"), *OtherActor->GetName());
	
	GelEffectInfo targetInfo = GelEffectInfo();
	FVector empty = FVector::ZeroVector;
	target->GetMovementInfo(targetInfo, empty);
	OriginGelEffectInfo originInfo = OriginGelEffectInfo();
	target->GetOriginMovementInfo(originInfo);
	
	targetInfo.targetAcceleration = originInfo.originAcceleration;
	targetInfo.targetMoveSpeed = originInfo.originMoveSpeed;
	targetInfo.targetFriction = originInfo.originFriction;

	target->SetMovementInfo(targetInfo);
}

void AActionPlane::RepulsionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IGelEffectTarget* target = ConvertActorToGelEffectTarget(OtherActor);
	if (nullptr == target)
		return;

	target->IncrementEffectCount(GelEffectType::JumpUp);
	int effectCount = target->GetEffectCount(GelEffectType::JumpUp);
	if (effectCount > 1)
	{
		return;
	}
	if (effectCount < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Error!!!!! Invalid Count!"))
		return;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Repulsion"));
	
	GelEffectInfo targetInfo = GelEffectInfo();
	FVector incidence = FVector::ZeroVector;
	target->GetMovementInfo(targetInfo, incidence);

	//내적을 하여, 입사 벡터에서 평면의 forwardvector요소만 뽑아내기
	FVector normal = GetActorForwardVector();

	//FVector incidence = targetInfo.targetVelocity;
	UE_LOG(LogTemp, Error, TEXT("%f"), incidence.X);
	UE_LOG(LogTemp, Error, TEXT("%f"), incidence.Y);
	UE_LOG(LogTemp, Error, TEXT("%f"), incidence.Z);
	
	float dot = FVector::DotProduct(normal, incidence);
	dot = FMath::Abs(dot);

	//반발하는지 여부 체크
	bool bReflection = false;

	//너무 작으면 다음 태스크. 충분히 크면, 반발 태스크 진행.
	if (dot > 100.f)
	{
		//반발할 크기는 최솟값을 보장함.
		dot = FMath::Max(dot, 500.f);
		//반발 시, 뽑아낸 것 만큼 감쇠하고, 반사하기에 2배
		FVector reflect = GetActorForwardVector() * dot * 2.f;
		//기존 이동 속도에 합산.
		reflect += incidence;
		//속도 갱신
		targetInfo.targetVelocity = reflect;
		//반발하니 반발여부 체크.
		bReflection = true;
		
		UE_LOG(LogTemp, Error, TEXT("%f"), reflect.X);
		UE_LOG(LogTemp, Error, TEXT("%f"), reflect.Y);
		UE_LOG(LogTemp, Error, TEXT("%f"), reflect.Z);
	}

	targetInfo.targetJumpPower *= 2;
	//반발여부를 같이 넘겨줌.
	target->SetMovementInfo(targetInfo, bReflection);
}

void AActionPlane::RepulsionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IGelEffectTarget* target = ConvertActorToGelEffectTarget(OtherActor);
	if (nullptr == target)
		return;

	target->DecrementEffectCount(GelEffectType::JumpUp);
	int effectCount = target->GetEffectCount(GelEffectType::JumpUp);
	if (effectCount > 0)
	{
		return;
	}
	if (effectCount < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Error!!!!! Invalid Count!"))
		return;
	}
	
	UE_LOG(LogTemp, Error, TEXT("RepulsionEnd"));
	UE_LOG(LogTemp, Error, TEXT("%s"), *OtherActor->GetName());
	
	GelEffectInfo targetInfo = GelEffectInfo();
	FVector empty = FVector::ZeroVector;
	target->GetMovementInfo(targetInfo, empty);
	OriginGelEffectInfo originInfo = OriginGelEffectInfo();
	target->GetOriginMovementInfo(originInfo);

	targetInfo.targetJumpPower = originInfo.originJumpPower;
	target->SetMovementInfo(targetInfo);
}

class IGelEffectTarget* AActionPlane::ConvertActorToGelEffectTarget(AActor* actor)
{
	return Cast<IGelEffectTarget>(actor);
}
