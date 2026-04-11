// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "CPlayerAnimInstance.h"
#include "ActionSystem/CActionComponent.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "CGameplayTags.h"
#include "CAttributeComponent.h"

ACPlayerCharacter::ACPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// don't rotate when the controller rotates. let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// ====== Camera Boom Component ======
	// create a camera boom (pulls in towards the player if there is a collision)
	CameraBoomComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomComponent"));
	CameraBoomComponent->SetupAttachment(RootComponent);
	CameraBoomComponent->TargetArmLength = 200.0f;
	CameraBoomComponent->bUsePawnControlRotation = true;

	// ====== Follow Camera Component ======
	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCameraComponent"));
	FollowCameraComponent->SetupAttachment(CameraBoomComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	// ====== Action Component ======
	ActionComponent = CreateDefaultSubobject<UCActionComponent>(TEXT("ActionComponent"));

	// ====== Weapon Slots Component ======
	WeaponSlotsComponent = CreateDefaultSubobject<UCWeaponSlotsComponent>(TEXT("WeaponSlotsComponent"));

	// ====== Attribute Component ======
	AttributeComponent = CreateDefaultSubobject<UCAttributeComponent>(TEXT("AttributeComponent"));

	// mouse sensitivity
	NormalTurnRate = 1.0f;
	NormalLookUpRate = 1.0f;
	AimTurnRate = NormalTurnRate;
	AimLookUpRate = NormalLookUpRate;
	MouseXSensitivity = 0.0f;
	MouseYSensitivity = 0.0f;

	// set aiming setting
	bIsAiming = false;

	/// \NOTE: camera's aim FOV is set in CAction_AimCamera;
}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// jumping
		EnhancedInputComponent->BindAction(InputActions.JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(InputActions.JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// moving
		EnhancedInputComponent->BindAction(InputActions.MoveAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Move);

		// looking
		EnhancedInputComponent->BindAction(InputActions.MouseLookAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InputActions.LookAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Look);

		// aiming
		EnhancedInputComponent->BindAction(InputActions.AimAction, ETriggerEvent::Started, this, &ACPlayerCharacter::Aim);
		EnhancedInputComponent->BindAction(InputActions.AimAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::Aim);

		// firing
		EnhancedInputComponent->BindAction(InputActions.FireAction, ETriggerEvent::Started, this, &ACPlayerCharacter::Fire);

		// reloading
		EnhancedInputComponent->BindAction(InputActions.ReloadAction, ETriggerEvent::Started, this, &ACPlayerCharacter::Reload);

		// sprinting
		EnhancedInputComponent->BindAction(InputActions.SprintAction, ETriggerEvent::Started, this, &ACPlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(InputActions.SprintAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::Sprint);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// initialize animation instance
	PlayerAnimInstance = Cast<UCPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	WeaponSlotsComponent->SpawnWeapon();
}

void ACPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ACPlayerCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ACPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ACPlayerCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController())
	{
		MouseXSensitivity = bIsAiming ? AimTurnRate : NormalTurnRate;
		MouseYSensitivity = bIsAiming ? AimLookUpRate : NormalLookUpRate;

		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw * MouseXSensitivity);
		AddControllerPitchInput(Pitch * MouseYSensitivity);
	}
}

void ACPlayerCharacter::Aim(const FInputActionValue& Value)
{
	bIsAiming = Value.Get<bool>();

	if (bIsAiming)
	{
		ActionComponent->StartActionByTag(this, CGameplayTags::AimAction);
	}
	else
	{
		ActionComponent->StopActionByTag(this, CGameplayTags::AimAction);
	}
}

void ACPlayerCharacter::Fire(const FInputActionValue& Value)
{
	ActionComponent->StartActionByTag(this, CGameplayTags::FireAction);
}

void ACPlayerCharacter::Reload(const FInputActionValue& Value)
{
	ActionComponent->StartActionByTag(this, CGameplayTags::ReloadAction);
}

void ACPlayerCharacter::Sprint(const FInputActionValue& Value)
{
	bIsSprinting = Value.Get<bool>();
	
	if (bIsSprinting)
	{
		ActionComponent->StartActionByTag(this, CGameplayTags::SprintAction);
	}
	else
	{
		ActionComponent->StopActionByTag(this, CGameplayTags::SprintAction);
	}
}
