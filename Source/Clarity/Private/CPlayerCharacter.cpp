// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CWeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "CPlayerAnimInstance.h"

ACPlayerCharacter::ACPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponSocketName = FName(TEXT("RightHandSocket"));
	BarrelSocketName = FName(TEXT("MuzzleFlash"));

	// set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// don't rotate when the controller rotates. let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	/// \NOTE: sets defaults, better tweak it in derived blueprint
	DefaultWalkingSpeed = 300.f;
	AimingWalkingSpeed = 200.f;

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkingSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// create a camera boom (pulls in towards the player if there is a collision)
	CameraBoomComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomComponent"));
	CameraBoomComponent->SetupAttachment(RootComponent);
	CameraBoomComponent->TargetArmLength = 200.0f;
	CameraBoomComponent->bUsePawnControlRotation = true;

	// create a follow camera
	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCameraComponent"));
	FollowCameraComponent->SetupAttachment(CameraBoomComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	// mouse sensitivity
	NormalTurnRate = 1.0f;
	NormalLookUpRate = 1.0f;
	AimTurnRate = NormalTurnRate;
	AimLookUpRate = NormalLookUpRate;
	MouseXSensitivity = 0.0f;
	MouseYSensitivity = 0.0f;

	// set aiming setting
	bIsAiming = false;

	// set camera FOV settings
	CameraDefaultFOV = FollowCameraComponent->FieldOfView;
	CameraCurrentFOV = CameraDefaultFOV;
	CameraAimingFOV = 40.0f;
	CameraAimingSpeed = 20.0f;
}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Move);

		// looking
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Look);

		// aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ACPlayerCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::Aim);

		// firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACPlayerCharacter::Fire);
		
		/// \NOTE: if needed the full auto
		//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::Fire);
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

	SpawnWeapon();
}

void ACPlayerCharacter::PlayFireSound()
{
	if (FiringAudio)
	{
		UGameplayStatics::PlaySound2D(this, FiringAudio);
	}
}

void ACPlayerCharacter::PlayMuzzleFlash(const FTransform SocketTransform)
{
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
	}
}

void ACPlayerCharacter::PlayImpactEffect(const FVector& ImpactPoint)
{
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactPoint);
	}
}

void ACPlayerCharacter::PlayWeaponRecoil()
{
	if (ensure(PlayerAnimInstance))
	{
		PlayerAnimInstance->DoProceduralRecoil(1.5f);
	}
}

void ACPlayerCharacter::SpawnWeapon()
{
	CurrentWeapon = GetWorld()->SpawnActor<ACWeaponBase>(DefaultWeapon);

	if (CurrentWeapon)
	{
		const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName(WeaponSocketName);
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(Cast<AActor>(CurrentWeapon), GetMesh());
		}
	}
}

bool ACPlayerCharacter::GetCrosshairWorldProperties(FVector& WorldPosition, FVector& WorldDirection)
{
	FVector2D ViewportSize;

	// get screen size
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// calculate crosshair location on screen (in the middle)
	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// try to convert data from screen to world and save to WorldPosition and WorldDirection
	return UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, WorldPosition, WorldDirection);
}

void ACPlayerCharacter::SetAimingFOV(bool IsAiming)
{
	TargetFOV = IsAiming ? CameraAimingFOV : CameraDefaultFOV;

	GetWorldTimerManager().SetTimer(AimingTimerHandle, this, &ACPlayerCharacter::UpdateFOV, GetWorld()->GetDeltaSeconds(), true);
}

void ACPlayerCharacter::UpdateFOV()
{
	CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, TargetFOV, GetWorld()->GetDeltaSeconds(), CameraAimingSpeed);
	FollowCameraComponent->SetFieldOfView(CameraCurrentFOV);

	if (FMath::IsNearlyEqual(CameraCurrentFOV, TargetFOV, 0.1f))
	{
		GetWorldTimerManager().ClearTimer(AimingTimerHandle);
	}
}

void ACPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ACPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ACPlayerCharacter::Aim(const FInputActionValue& Value)
{
	bIsAiming = Value.Get<bool>();

	GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimingWalkingSpeed : DefaultWalkingSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = bIsAiming ? false : true;
	GetCharacterMovement()->bUseControllerDesiredRotation = bIsAiming ? true : false;

	SetAimingFOV(bIsAiming);
}

void ACPlayerCharacter::Fire(const FInputActionValue& Value)
{
	PlayFireSound();

	const USkeletalMeshSocket* BarrelSocket = CurrentWeapon->GetMesh()->GetSocketByName(BarrelSocketName);

	if (!BarrelSocket) return;

	const FTransform SocketTransform = BarrelSocket->GetSocketTransform(CurrentWeapon->GetMesh());

	PlayMuzzleFlash(SocketTransform);

	bScreenToWorld = GetCrosshairWorldProperties(CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult HitResult;
		FVector Start = CrosshairWorldPosition;

		/// \TODO: fix the magic number
		FVector End = Start + CrosshairWorldDirection * 60000.0f;

		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			PlayImpactEffect(HitResult.Location);

			//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
			//DrawDebugPoint(GetWorld(), HitResult.Location, 4.0f, FColor::Blue, false, 2.0f);
		}

		FHitResult WeaponTraceHit;

		const FVector WeaponStart = SocketTransform.GetLocation();
		const FVector WeaponEnd = HitResult.Location;

		GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponStart, WeaponEnd, ECollisionChannel::ECC_Visibility);
		if (WeaponTraceHit.bBlockingHit)
		{
			PlayImpactEffect(WeaponTraceHit.Location);

			//DrawDebugLine(GetWorld(), WeaponStart, WeaponTraceHit.Location, FColor::Red, false, 2.0f);
			//DrawDebugPoint(GetWorld(), WeaponTraceHit.Location, 4.0f, FColor::Blue, false, 2.0f);
		}
	}

	PlayWeaponRecoil();
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

void ACPlayerCharacter::DoJumpStart()
{
	Jump();
}

void ACPlayerCharacter::DoJumpEnd()
{
	StopJumping();
}




