// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GelEffectTarget.h"
#include "MovableTarget.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PORTAL1POINT5_API APlayerCharacter : public ACharacter, public IMovableTarget, public IGelEffectTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//인터페이스 함수
public:
	//IMovableTarget
	
	virtual FVector GetLocation() override;
	virtual void SetLocation(FVector newLocation) override;
	
	virtual FVector GetVelocity() override;
	virtual void SetVelocity(FVector newVelocity) override;

	virtual FRotator GetRotation() override;
	virtual void SetRotation(FRotator newRotation) override;

	//IGelEffectTarget
	

	
	//멤버변수
private:

	//==이동 관련==
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IMC_PlayerCharacter;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Look;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Jump;
	
	//이동 방향
	FVector Direction;

	//이전 속도
	FVector BeforeVelocity;

	//==포탈 설치==
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_ShootBluePortal;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_ShootOrangePortal;
	
	class APortalManager* PortalManager;
	
public:
	UPROPERTY(EditAnywhere)
	class UCameraComponent* PlayerCameraComp;

	//함수
private:
	//==이동 액션 함수==
	void OnActionMove(const FInputActionValue& value);
	void OnActionLook(const FInputActionValue& value);
	void OnActionJump(const FInputActionValue& value);
	//==포탈 액션 함수==
	void OnActionShootBluePortal(const FInputActionValue& value);
	void OnActionShootOrangePortal(const FInputActionValue& value);
	
public:
	void SetPortalManager(class APortalManager* portalManager);
	
	
};
