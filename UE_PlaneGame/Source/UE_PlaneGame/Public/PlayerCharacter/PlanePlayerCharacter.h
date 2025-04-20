// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/GrabHandle.h"
#include "GameFramework/Character.h"
#include "PlanePlayerCharacter.generated.h"


class AWorldButton;
class UTurbulenceReciever;
class APlayerHand;
class AWorldItem;
class UTextRenderComponent;
struct FInputActionValue;
class APlanePlayerController;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class UE_PLANEGAME_API APlanePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlanePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//helper

protected:
	APlanePlayerController* MyPlayerController;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
//Components
	
	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadOnly)
	UCameraComponent* FirstPersonCamera;
public:
	UCameraComponent* GetCamera() const {return this->FirstPersonCamera;}
protected:
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UChildActorComponent* PlayerHandCA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OriginalHandPosition;
	
private:
	APlayerHand* GetPlayerHand();
	
	//Basic Input
protected:
	UPROPERTY(EditAnywhere, Category = "Input | Mapping")
	UInputMappingContext* BasicCharacterInputMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Input | Mapping")
	UInputMappingContext* MoveHandMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input | Mapping")
	UInputMappingContext* MoveCameraMappingContext;

private:
	void AddMappingContext(UInputMappingContext* MappingContextToAdd);
	void RemoveMappingContext(UInputMappingContext* MappingContextToRemove);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Input|Action|Move")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input|Action")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input|Action|Move")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input|Action|HandMovement")
	UInputAction* ToggleHandMovementAction;
	
	UPROPERTY(EditAnywhere, Category = "Input|Action|HandMovement")
	UInputAction* HandMovementAction;

	UPROPERTY(EditAnywhere, Category = "Input|Action|HandMovement")
	UInputAction* HandTurnAction;

	UPROPERTY(EditAnywhere, Category = "Input|Action")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input|Action")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input|Action")
	UInputAction* ThrowAction;

	//process input

	//Moving
private:
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void StartCrouch();
	void EndCrouch();


	
	//Hand Movement
	
	void Local_CalculateHandMovement(const FInputActionValue& Value);
	
	void LocalCalculateHandRotation(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void Server_ApplyHandRotation(FRotator DeltaRotation);
	void Server_ApplyHandRotation_Implementation(FRotator DeltaRotation);
	
	void ActivateHandMovement();
	void DeactivateHandMovement();

	//Interact

	void Interact();
	//Grab and let go

	UFUNCTION(Server, Unreliable)
	void Server_SingleInteract(AInteractable* Interactable);
	void Server_SingleInteract_Implementation(AInteractable* Interactable);
	

	UFUNCTION(Server, Unreliable)
	void Server_HoldInteract_Start(AInteractable* Interactable);
	void Server_HoldInteract_Start_Implementation(AInteractable* Interactable);


	UFUNCTION(Server, Unreliable)
	void Server_HoldInteract_End();
	void Server_HoldInteract_End_Implementation();

	AInteractable* CurrentHoldInteractable;

	UFUNCTION(Server, Unreliable)
	void Server_LetGo();
	void Server_LetGo_Implementation();
	
public:
	
	void PickUpItem(AWorldItem* Item);

public:
	void GrabHandle(AGrabHandle* Handle);
	

private:	
	void DropItem();
	void LetHandleGo();


	
	UPROPERTY(Replicated)
	AWorldItem* R_CurrentyHeldWorldItem=nullptr;

	UPROPERTY(Replicated)
	AGrabHandle* R_CurrentlyHeldGrabHandle =nullptr;
	
	
	//for moving hand back
	bool bMovingHand=false;

	UFUNCTION(Server, Unreliable)
	void Server_Tick_MoveHandBack();
	void Server_Tick_MoveHandBack_Implementation();
	
	
	void NotifyToolHandMovement(const FVector& MovementVector);


	//Throw
	void StartThrow();
	
	UFUNCTION(Server, Reliable)
	void Server_Throw(FVector ThrowVector);
	void Server_Throw_Implementation(FVector ThrowVector);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ThrowStrength=10000;
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Action|Move")
	float MovementSpeed =300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Action|HandMovement")
	float HandMovementSpeed =10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Input|Action|HandMovement")
	float HandTurnSpeed =60;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Input|Action|HandMovement")
	float CameraMoveDistanceThreshold =35;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Action|HandMovement")
	float HandMovementCameraSpeed = 0.5f;

	//Visuals on other machines

private:
	UFUNCTION(Server, Unreliable)
	void Server_Tick_UpdateVisualHandTransform(FTransform HandWorldTransform);
	void Server_Tick_UpdateVisualHandTransform_Implementation(FTransform HandWorldTransform);
	

	UFUNCTION(NetMulticast, Unreliable)
	void MC_ApplyVisualHandPosition(FTransform HandWorldTransform);
	void MC_ApplyVisualHandPosition_Implementation(FTransform HandWorldTransform);
	
	FTransform VisualNonLocalHandPTransform;

public:
	//To use in anim bp for the player hand pos
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetHandWorldTransform();
	
	
};
