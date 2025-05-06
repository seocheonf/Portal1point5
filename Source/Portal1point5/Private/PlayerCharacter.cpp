// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PortalManager.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCameraComp"));
	PlayerCameraComp->SetupAttachment(RootComponent);
	PlayerCameraComp->bEditableWhenInherited = true;
	
	PlayerCameraComp->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	PlayerCameraComp->bUsePawnControlRotation = true;
	
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (auto* pc = Cast<APlayerController>(Controller))
	{
		auto* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		subsystem->RemoveMappingContext(IMC_PlayerCharacter);
		subsystem->AddMappingContext(IMC_PlayerCharacter, 0);
	}
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	FTransform t = FTransform(rot);

	Direction = t.TransformVector(Direction);
	AddMovementInput(Direction);

	Direction = FVector::ZeroVector;

	BeforeVelocity = GetCharacterMovement()->Velocity;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//==Move==
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::OnActionLook);
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnActionMove);
		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::OnActionJump);
		//==Portal==
		input->BindAction(IA_ShootBluePortal, ETriggerEvent::Started, this, &APlayerCharacter::OnActionShootBluePortal);
		input->BindAction(IA_ShootOrangePortal, ETriggerEvent::Started, this, &APlayerCharacter::OnActionShootOrangePortal);
	}
}

void APlayerCharacter::OnActionMove(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Z = 0.f;
	Direction.Normalize();
}

void APlayerCharacter::OnActionLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	AddControllerYawInput(v.X);
	AddControllerPitchInput(v.Y);
}

void APlayerCharacter::OnActionJump(const FInputActionValue& value)
{
	Jump();
}

void APlayerCharacter::OnActionShootBluePortal(const FInputActionValue& value)
{
	PortalManager->PutBluePortal(this, PlayerCameraComp->GetComponentLocation(), PlayerCameraComp->GetForwardVector());
}

void APlayerCharacter::OnActionShootOrangePortal(const FInputActionValue& value)
{
	PortalManager->PutOrangePortal(this, PlayerCameraComp->GetComponentLocation(), PlayerCameraComp->GetForwardVector());
}

void APlayerCharacter::SetPortalManager(class APortalManager* portalManager)
{
	PortalManager = portalManager;
}

FVector APlayerCharacter::GetVelocity()
{
	return BeforeVelocity;
}

void APlayerCharacter::SetVelocity(FVector newVelocity)
{
	GetCharacterMovement()->Velocity = newVelocity;
}

FRotator APlayerCharacter::GetRotation()
{
	return PlayerCameraComp->GetComponentRotation();
}

void APlayerCharacter::SetRotation(FRotator newRotation)
{
	//Controller->SetControlRotation(FRotator(0, 0, 0));
	Controller->SetControlRotation(FRotator(newRotation.Pitch, newRotation.Yaw, 0.f));
	
	// FRotator currentRot = Controller->GetControlRotation();
	// FRotator addRot = newRotation - currentRot;
	// AddControllerYawInput(addRot.Yaw);
	// AddControllerPitchInput(addRot.Pitch);
}

