// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_PlaneGame/Public/PlayerCharacter/PlanePlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "PlayerCharacter/PlanePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Debug.h"
#include "Items/WorldItem.h"
#include "PlayerCharacter/PlayerHand.h"


// Sets default values
APlanePlayerCharacter::APlanePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;
	
	PlayerHandCA= CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	PlayerHandCA->SetupAttachment(FirstPersonCamera);
}

// Called when the game starts or when spawned
void APlanePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->MyPlayerController= Cast<APlanePlayerController>(GetController());

	this->AddMappingContext(this->BasicCharacterInputMappingContext);
	this->AddMappingContext(MoveCameraMappingContext);

	this->OriginalHandPosition= this->PlayerHandCA->GetRelativeLocation();//gets remembered to move back later
}

// Called every frame
void APlanePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bMovingHand)
		Tick_MoveHandBack();
}

// Called to bind functionality to input
void APlanePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EIC)
		return;

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlanePlayerCharacter::Move);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlanePlayerCharacter::Look);
	EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlanePlayerCharacter::Jump);

	EIC->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlanePlayerCharacter::StartCrouch);
	EIC->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlanePlayerCharacter::EndCrouch);
	
	EIC->BindAction(GrabAction, ETriggerEvent::Completed, this, &APlanePlayerCharacter::Grab);
	
	EIC->BindAction(HandMovementAction, ETriggerEvent::Triggered, this, &APlanePlayerCharacter::MoveHand);
	EIC->BindAction(HandTurnAction, ETriggerEvent::Triggered, this, &APlanePlayerCharacter::TurnHand);
	EIC->BindAction(ToggleHandMovementAction, ETriggerEvent::Started, this, &APlanePlayerCharacter::ActivateHandMovement);
	EIC->BindAction(ToggleHandMovementAction, ETriggerEvent::Completed, this, &APlanePlayerCharacter::DeactivateHandMovement);
	
}

APlayerHand* APlanePlayerCharacter::GetPlayerHand()
{
	return Cast<APlayerHand>(this->PlayerHandCA->GetChildActor());
}

void APlanePlayerCharacter::AddMappingContext(UInputMappingContext* MappingContextToAdd)
{
	if(!IsLocallyControlled())
		return;
	
	ULocalPlayer* LocalPlayer= MyPlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	EnhancedInputSubSystem->AddMappingContext(MappingContextToAdd,0);	
}

void APlanePlayerCharacter::RemoveMappingContext(UInputMappingContext* MappingContextToRemove)
{
	ULocalPlayer* LocalPlayer= MyPlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	EnhancedInputSubSystem->RemoveMappingContext(MappingContextToRemove);	
}

void APlanePlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector= Value.Get<FVector2D>();
	MoveVector.Normalize();
	
	FVector MovementInputVector = GetActorForwardVector() * MoveVector.X + GetActorRightVector() * MoveVector.Y; 
	AddMovementInput(MovementInputVector);
}

void APlanePlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D DeltaLook= Value.Get<FVector2D>();

	AddControllerPitchInput(DeltaLook.Y*-1);
	AddControllerYawInput(DeltaLook.X);
}

void APlanePlayerCharacter::StartCrouch()
{
	this->Crouch();
}

void APlanePlayerCharacter::EndCrouch()
{
	this->UnCrouch();
}

void APlanePlayerCharacter::MoveHand(const FInputActionValue& Value)
{
	FVector2D VectorValue = Value.Get<FVector2D>();
	FVector MovementVector = FVector(0, VectorValue.X, VectorValue.Y);
	MovementVector *= HandMovementSpeed * GetWorld()->DeltaTimeSeconds;

	FVector PredictedPosition=PlayerHandCA->GetRelativeLocation()+MovementVector;
	PredictedPosition*=FVector(0,1,1);//ignore Depth
	Debug::Print("Hand distance to center: "+ FString::SanitizeFloat(PredictedPosition.Length()),GetWorld()->DeltaTimeSeconds);
	if(PredictedPosition.Length()<this->CameraMoveDistanceThreshold)
	{
		this->PlayerHandCA->AddRelativeLocation(MovementVector);	
	}
	else
	{
		FVector2D DeltaLook = Value.Get<FVector2D>();
		DeltaLook*=this->HandMovementCameraSpeed;

		AddControllerPitchInput(DeltaLook.Y * -1);
		AddControllerYawInput(DeltaLook.X);
	}
	
	
}

void APlanePlayerCharacter::TurnHand(const FInputActionValue& Value)
{
	float FValue=Value.Get<float>();
	FRotator DeltaRotaion= FRotator(0,0,FValue*HandTurnSpeed*GetWorld()->DeltaTimeSeconds);
	PlayerHandCA->AddLocalRotation(DeltaRotaion);
}

void APlanePlayerCharacter::ActivateHandMovement()
{
	this->bMovingHand=true;
	this->AddMappingContext(MoveHandMappingContext);
	this->RemoveMappingContext(MoveCameraMappingContext);
}

void APlanePlayerCharacter::DeactivateHandMovement()
{
	this->bMovingHand=false;
	this->RemoveMappingContext(MoveHandMappingContext);
	this->AddMappingContext(MoveCameraMappingContext);
}

void APlanePlayerCharacter::Grab()
{
	if(CurrentyHeldWorldItem)
		LetGo();
	else
		PickUp();
}

void APlanePlayerCharacter::PickUp()
{
	AWorldItem* OverlappingItem = this->GetPlayerHand()->GetOverlappingItem();
	if (!OverlappingItem)
		return;
	
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget, // Location
		EAttachmentRule::SnapToTarget, // Rotation
		EAttachmentRule::KeepWorld, // Scale
		true // Weld simulated bodies
	);

	OverlappingItem->AttachToComponent(this->PlayerHandCA, AttachRules);
	if(UPrimitiveComponent* PhysicsComponent=Cast<UPrimitiveComponent>(OverlappingItem->GetRootComponent()))
	{
		PhysicsComponent->SetSimulatePhysics(false);
		PhysicsComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
	CurrentyHeldWorldItem=OverlappingItem;
	
	UTurbulenceReciever* TurbulenceReciever= Cast<UTurbulenceReciever>(CurrentyHeldWorldItem->GetComponentByClass(UTurbulenceReciever::StaticClass()));
	if(TurbulenceReciever)
	{
		TurbulenceReciever->bActive=false;
	}
}

void APlanePlayerCharacter::LetGo()
{
	if(!CurrentyHeldWorldItem)
		return;
	
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	CurrentyHeldWorldItem->DetachFromActor(DetachRules);
	if(UPrimitiveComponent* PhysicsComponent=Cast<UPrimitiveComponent>(CurrentyHeldWorldItem->GetRootComponent()))
	{
		PhysicsComponent->SetSimulatePhysics(true);
		PhysicsComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
	
	UTurbulenceReciever* TurbulenceReciever= Cast<UTurbulenceReciever>(CurrentyHeldWorldItem->GetComponentByClass(UTurbulenceReciever::StaticClass()));
	if(TurbulenceReciever)
	{
		TurbulenceReciever->bActive=true;
	}
	
	CurrentyHeldWorldItem=nullptr;
	
}

void APlanePlayerCharacter::Tick_MoveHandBack()
{
	this->PlayerHandCA->SetRelativeLocation(FMath::VInterpTo(this->PlayerHandCA->GetRelativeLocation(),this->OriginalHandPosition,GetWorld()->DeltaTimeSeconds,1));
}





