// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "SInteractionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//The camera's arm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent); // The other option is attach to mesh/bone is: GetMesh(), "head" ?EyeLocation
	SpringArmComp->TargetArmLength = ZoomDefault;

	//The camera itself
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Input component setup
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Directional movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	//Directional Camera
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//Primary Attack
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	//Interact
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	//Jump action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::StopJump);
	//Camera Control by buttons
	PlayerInputComponent->BindAction("ZoomIn", IE_Repeat, this, &ASCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Repeat, this, &ASCharacter::ZoomOut);
	//Camera Control by scroll
	PlayerInputComponent->BindAxis("CameraZoom", this, &ASCharacter::CameraZoom);
}

void ASCharacter::MoveForward(float Value)
{
	//Foward and backward movement
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	//Lateral movement
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	/*Unreal's coordinates:
	* X = Forward (Red)
	* Y = Right (Green)
	* Z = Up (Blue)
	*/

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::Jump()
{
	Super::Jump();
	bPressedJump = true;
}

void ASCharacter::StopJump()
{
	bPressedJump = false;
}

void ASCharacter::PrimaryAttack()
{
	//Selecting the hand bone for the projectile
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	//Selecting the rotation of the projectile
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	//Spawning the projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryInteract()
{
	
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::ZoomIn()
{
	//SpringArmComp->TargetArmLength -= 5; // Raw input
	SpringArmComp->TargetArmLength -= ZoomValue; //Using preset values
}

void ASCharacter::ZoomOut()
{
	//SpringArmComp->TargetArmLength += 5; // Raw input
	SpringArmComp->TargetArmLength += ZoomValue; //Using preset values
}

void ASCharacter::CameraZoom(const float Value)
{
	if (Value == 0.f || !Controller) return;
	{
		const float NewZoom = SpringArmComp->TargetArmLength + (Value * ZoomValue);
		SpringArmComp->TargetArmLength = FMath::Clamp(NewZoom, ZoomMin, ZoomMax);
	}
}
