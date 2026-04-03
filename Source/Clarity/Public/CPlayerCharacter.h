// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class ACWeaponBase;
class USoundCue;
class UParticleSystem;
class UCPlayerAnimInstance;

UCLASS(Abstract)
class CLARITY_API ACPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* CameraBoomComponent;

	/** follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FollowCameraComponent;

	/** jump input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** move input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** look input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** mouse look input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	/** aim input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimAction;

	/** shoot input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UCPlayerAnimInstance* PlayerAnimInstance;

	/* aiming a gun */
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	bool bIsAiming;

	/* FOV setting for default */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraDefaultFOV;

	/* FOV setting for current */
	float CameraCurrentFOV;

	/* FOV setting for aiming */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraAimingFOV;

	/* control interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraAimingSpeed;
	
	/* default horizontal rate sensitivity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float NormalTurnRate;

	/* default vertical rate sensitivity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float NormalLookUpRate;

	/* aim horizontal rate sensitivity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "1.0"), Category = "Camera")
	float AimTurnRate;

	/* aim vertical rate sensitivity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "1.0"), Category = "Camera")
	float AimLookUpRate;

	/* current horizontal sensitivity */
	float MouseXSensitivity;

	/* current vertical sesitivity */
	float MouseYSensitivity;

	/* target FOV to zoom into */
	float TargetFOV;

	/* timer to handle zooming */
	FTimerHandle AimingTimerHandle;

	/* walking speed when aiming */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float AimingWalkingSpeed;

	/* default speed when aiming */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float DefaultWalkingSpeed;

	ACWeaponBase* CurrentWeapon;

	FName WeaponSocketName;

	FName BarrelSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ACWeaponBase> DefaultWeapon;

	FVector CrosshairWorldPosition;

	FVector CrosshairWorldDirection;

	/* is crosshair translated successfully */
	bool bScreenToWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	USoundCue* FiringAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	UAnimMontage* FireMontage;

	virtual void BeginPlay() override;

	void PlayFireSound();

	void PlayMuzzleFlash(const FTransform SocketTransform);

	void PlayImpactEffect(const FVector& ImpactPoint);

	void PlayWeaponRecoil();

	void SpawnWeapon();

	/* converts crosshair screen position to world position (and direction)*/
	bool GetCrosshairWorldProperties(FVector& WorldPosition, FVector& WorldDirection);

	/* called for aiming timer */
	void SetAimingFOV(bool IsAiming);

	/* interpolates the FOV */
	void UpdateFOV();

	/** called for movement input */
	void Move(const FInputActionValue& Value);

	/** called for looking input */
	void Look(const FInputActionValue& Value);

	/** called for aiming input */
	void Aim(const FInputActionValue& Value);

	/** called for firing input */
	void Fire(const FInputActionValue& Value);

public:
	ACPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	/** returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoomComponent; }

	/** returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCameraComponent; }
};
