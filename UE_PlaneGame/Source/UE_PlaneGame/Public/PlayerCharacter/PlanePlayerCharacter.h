// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabHandle.h"
#include "IGrabHandleActor.h"
#include "GameFramework/Character.h"
#include "PlanePlayerCharacter.generated.h"


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
	
//Components
	
	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadOnly)
	UCameraComponent* FirstPersonCamera;

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
	UInputAction* GrabAction;

	UPROPERTY(EditAnywhere, Category = "Input|Action")
	UInputAction* CrouchAction;

	//process input

	//Moving
private:
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void StartCrouch();
	void EndCrouch();

	//Hand Movement
	void MoveHand(const FInputActionValue& Value);
	void TurnHand(const FInputActionValue& Value);
	
	void ActivateHandMovement();
	void DeactivateHandMovement();

	void Grab();
	void PickUp();
	void LetGo();
	AWorldItem* CurrentyHeldWorldItem=nullptr;
	IIGrabHandleActor* CurrentGrabHandleActor=nullptr;
	AGrabHandle* GrabHandle =nullptr;

	//for moving hand back
	bool bMovingHand=false;
	
	void Tick_MoveHandBack();
	
	void NotifyToolHandMovement(const FVector& MovementVector);
	
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
};
