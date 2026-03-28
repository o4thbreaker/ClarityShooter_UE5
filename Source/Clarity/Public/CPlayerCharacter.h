// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

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

	/* aiming a gun */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bIsAiming;

	/* FOV setting for default */
	float CameraDefaultFOV;

	/* FOV setting for current */
	float CameraCurrentFOV;

	/* FOV setting for aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	float CameraAimingFOV;

	/* control interpolation speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	float CameraAimingSpeed;

	/** called for movement input */
	void Move(const FInputActionValue& Value);

	/** called for looking input */
	void Look(const FInputActionValue& Value);

	/** called for aiming input */
	void Aim(const FInputActionValue& Value);

public:
	ACPlayerCharacter();

	virtual void Tick(float DeltaTime) override;

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
