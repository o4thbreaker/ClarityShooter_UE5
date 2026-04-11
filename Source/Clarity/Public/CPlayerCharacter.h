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
class UCPlayerAnimInstance;
class UCActionComponent;
class UCWeaponSlotsComponent;
class UCAttributeComponent;

USTRUCT(BlueprintType)
struct FCharacterInputActions
{
	GENERATED_BODY()

public:
	/** jump input action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	/** move input action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	/** look input action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	/** mouse look input action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MouseLookAction;

	/** aim input action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AimAction;

	/** shoot input action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* FireAction;

	/** reload input action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ReloadAction;

	/** sprint input action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SprintAction;
};

UCLASS(Abstract)
class CLARITY_API ACPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCWeaponSlotsComponent* WeaponSlotsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCAttributeComponent* AttributeComponent; 

	/** camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* CameraBoomComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FollowCameraComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UCPlayerAnimInstance* PlayerAnimInstance;

	UPROPERTY(EditAnywhere, Category = "Input")
	FCharacterInputActions InputActions;

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Sprint")
	bool bIsSprinting;

#pragma region Sensitivity Values
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
#pragma endregion

#pragma region Input Methods
	/** called for movement input */
	void Move(const FInputActionValue& Value);

	/** called for looking input */
	void Look(const FInputActionValue& Value);

	/// \TODO: separate logic from input (Move and DoMove as example)
	/** called for aiming input */
	void Aim(const FInputActionValue& Value);

	/// \TODO: separate logic from input (Move and DoMove as example)
	/** called for firing input */
	void Fire(const FInputActionValue& Value);

	/// \TODO: separate logic from input (Move and DoMove as example)
	/** called for firing input */
	void Reload(const FInputActionValue& Value);

	/// \TODO: separate logic from input (Move and DoMove as example)
	/** called for sprinting input */
	void Sprint(const FInputActionValue& Value);

#pragma endregion

	virtual void BeginPlay() override;

public:
	ACPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** handles move logic separate from input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** handles look logic separate from input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoomComponent; }

	/** returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCameraComponent; }

	/** returns PLAYER's anim instance **/
	FORCEINLINE UCPlayerAnimInstance* GetPlayerAnimInstance() const { return PlayerAnimInstance; }
};
