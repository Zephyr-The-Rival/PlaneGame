// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_PlaneGame/Public/PlayerCharacter/PlanePlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "PlayerCharacter/PlanePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Debug.h"
#include "IDetailTreeNode.h"
#include "Interactables/GrabHandle.h"
#include "Interactables/WorldButton.h"
#include "Interactables/Items/Tool.h"
#include "Interactables/Items/WorldItem.h"
#include "Net/UnrealNetwork.h"
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
	if(!bMovingHand && this->IsLocallyControlled())
		Server_Tick_MoveHandBack();

	if(this->IsLocallyControlled())
		Server_Tick_UpdateVisualHandTransform(GetPlayerHand()->GetAttachComponent()->GetComponentTransform());
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
	
	EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &APlanePlayerCharacter::Interact);
	EIC->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlanePlayerCharacter::Server_HoldInteract_End);
	
	EIC->BindAction(HandMovementAction, ETriggerEvent::Triggered, this, &APlanePlayerCharacter::Local_CalculateHandMovement);
	EIC->BindAction(HandTurnAction, ETriggerEvent::Triggered, this, &APlanePlayerCharacter::LocalCalculateHandRotation);
	EIC->BindAction(ToggleHandMovementAction, ETriggerEvent::Started, this, &APlanePlayerCharacter::ActivateHandMovement);
	EIC->BindAction(ToggleHandMovementAction, ETriggerEvent::Completed, this, &APlanePlayerCharacter::DeactivateHandMovement);

	EIC->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &APlanePlayerCharacter::StartThrow);
	
}

void APlanePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlanePlayerCharacter, R_CurrentyHeldWorldItem);
	DOREPLIFETIME(APlanePlayerCharacter, R_CurrentlyHeldGrabHandle);
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

void APlanePlayerCharacter::Local_CalculateHandMovement(const FInputActionValue& Value)
{
	if(!IsLocallyControlled())
		return;
	
	FVector2D VectorValue = Value.Get<FVector2D>();

	bool bHandIsMoving = !VectorValue.IsNearlyZero();
	
	FVector MovementVector = FVector(0, VectorValue.X, VectorValue.Y);
	MovementVector *= HandMovementSpeed * GetWorld()->DeltaTimeSeconds;

	FVector PredictedPosition=PlayerHandCA->GetRelativeLocation()+MovementVector;
	PredictedPosition*=FVector(0,1,1);//ignore Depth
	//Debug::Print("Hand distance to center: "+ FString::SanitizeFloat(PredictedPosition.Length()),GetWorld()->DeltaTimeSeconds);
	if(PredictedPosition.Length()<this->CameraMoveDistanceThreshold)
	{
		this->PlayerHandCA->AddRelativeLocation(MovementVector);
		if (bHandIsMoving)
		{
			NotifyToolHandMovement(MovementVector);
		}
	}
	else
	{
		FVector2D DeltaLook = Value.Get<FVector2D>();
		DeltaLook*=this->HandMovementCameraSpeed;

		AddControllerPitchInput(DeltaLook.Y * -1);
		AddControllerYawInput(DeltaLook.X);
	}
}


void APlanePlayerCharacter::LocalCalculateHandRotation(const FInputActionValue& Value)
{
	float FValue=Value.Get<float>();
	FRotator DeltaRotation= FRotator(0,0,FValue*HandTurnSpeed*GetWorld()->DeltaTimeSeconds);
	this->Server_ApplyHandRotation(DeltaRotation);
	
}

void APlanePlayerCharacter::Server_ApplyHandRotation_Implementation(const FRotator DeltaRotation)
{
	PlayerHandCA->AddLocalRotation(DeltaRotation);	
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
	if (R_CurrentyHeldWorldItem)
	{
		ATool* Tool = Cast<ATool>(R_CurrentyHeldWorldItem);
		if (Tool)
		{
			Tool->OnHandMovementStopped();
		}
	}
	this->RemoveMappingContext(MoveHandMappingContext);
	this->AddMappingContext(MoveCameraMappingContext);
}

void APlanePlayerCharacter::Interact()
{
	//When interact action starts
	//runs locally on controlled player
	//R_CurrentyHeldWorldItem gets set on server and is replicated so it works here.
	if (R_CurrentyHeldWorldItem || R_CurrentlyHeldGrabHandle)
	{
		Server_LetGo();
		return;
	}

	UObject* InteractableObject = this->GetPlayerHand()->GetOverlappingInteractable();

	AInteractable* Interactable= Cast<AInteractable>(InteractableObject);
	
	if (!InteractableObject) //if no item can be found in hand
		return;

	if(Interactable->bHoldToInteract)
		Server_HoldInteract_Start(Interactable);
	else
		Server_SingleInteract(Interactable);
	
}

void APlanePlayerCharacter::Server_SingleInteract_Implementation(AInteractable* Interactable)
{
	Interactable->Interact(this);
}

void APlanePlayerCharacter::Server_HoldInteract_Start_Implementation(AInteractable* Interactable)
{
	this->CurrentHoldInteractable=Interactable;
	CurrentHoldInteractable->HoldInteract_Start(this);
}

void APlanePlayerCharacter::Server_HoldInteract_End_Implementation()
{
	if(!CurrentHoldInteractable)
		return;
	
	CurrentHoldInteractable->HoldInteract_End(this);
	this->CurrentHoldInteractable=nullptr;
}


void APlanePlayerCharacter::Server_LetGo_Implementation()
{
	if(R_CurrentlyHeldGrabHandle)
	{
		this->LetHandleGo();
		return;
	}
	if(R_CurrentyHeldWorldItem)
	{
		DropItem();
		return;
	}
}

void APlanePlayerCharacter::PickUpItem(AWorldItem* Item)
{
	if(!HasAuthority())
		return;
	
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget, // Location
		EAttachmentRule::SnapToTarget, // Rotation
		EAttachmentRule::KeepWorld, // Scale
		true // Weld simulated bodies
		);
		
	Item->AttachToComponent(this->GetPlayerHand()->GetAttachComponent(), AttachRules);
	R_CurrentyHeldWorldItem=Item;
	R_CurrentyHeldWorldItem->OnPickedUp_Server.Broadcast(); 
}

void APlanePlayerCharacter::GrabHandle(AGrabHandle* Handle)
{
	if(!HasAuthority())
		return;
	
	R_CurrentlyHeldGrabHandle = Handle;
	R_CurrentlyHeldGrabHandle->OnGrabbed.Broadcast(this->GetPlayerHand(), this);
}

void APlanePlayerCharacter::DropItem()
{
	if(!HasAuthority())
		return;
	
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	R_CurrentyHeldWorldItem->DetachFromActor(DetachRules);
	R_CurrentyHeldWorldItem->OnDropped_Server.Broadcast();
	R_CurrentyHeldWorldItem=nullptr;
}

void APlanePlayerCharacter::LetHandleGo()
{
	if(!HasAuthority())
		return;
	
	R_CurrentlyHeldGrabHandle->OnLetGo.Broadcast(this->GetPlayerHand(), this);
	R_CurrentlyHeldGrabHandle=nullptr;
}


void APlanePlayerCharacter::Server_Tick_MoveHandBack_Implementation()
{
	this->PlayerHandCA->SetRelativeLocation(FMath::VInterpTo(this->PlayerHandCA->GetRelativeLocation(),this->OriginalHandPosition,GetWorld()->DeltaTimeSeconds,1));
}

void APlanePlayerCharacter::NotifyToolHandMovement(const FVector& MovementVector)
{
	if (R_CurrentyHeldWorldItem)
	{
		ATool* Tool = Cast<ATool>(R_CurrentyHeldWorldItem);
		if (Tool)
		{
			Tool->OnHandMovement(MovementVector);
		}
	}
}

void APlanePlayerCharacter::StartThrow()
{
	this->Server_Throw(this->FirstPersonCamera->GetForwardVector());
}

void APlanePlayerCharacter::Server_Throw_Implementation(FVector ThrowVector)
{
	if(!this->R_CurrentyHeldWorldItem)
		return;

	AWorldItem* TmpItem = R_CurrentyHeldWorldItem;
	Server_LetGo_Implementation();

	UPrimitiveComponent* PhysicsComponent = Cast<UPrimitiveComponent>(TmpItem->GetRootComponent());
	PhysicsComponent->AddForce(ThrowVector * this->ThrowStrength* PhysicsComponent->GetMass());
}

void APlanePlayerCharacter::Server_Tick_UpdateVisualHandTransform_Implementation(FTransform HandWorldTransform)
{
	this->MC_ApplyVisualHandPosition(HandWorldTransform);
}

void APlanePlayerCharacter::MC_ApplyVisualHandPosition_Implementation(FTransform HandWorldTransform)
{
	if(IsLocallyControlled())//only for non controlled players
		return;

	this->VisualNonLocalHandPTransform=HandWorldTransform;
}

FTransform APlanePlayerCharacter::GetHandWorldTransform()
{
	if(IsLocallyControlled())
		return GetPlayerHand()->GetAttachComponent()->GetComponentTransform();
	else
		return VisualNonLocalHandPTransform;
}





