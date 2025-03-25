// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlanePlayerCharacter.generated.h"


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

//Components

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;
	
	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadOnly)
	UCameraComponent* FirstPersonCamera;
	
	//Basic Input
protected:
	UPROPERTY(EditAnywhere, Category = "Input | Mapping")
	UInputMappingContext* CharacterInputMapping;

	UPROPERTY(EditAnywhere, Category = "Input | Action")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input | Action")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input | Action")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input | Action")
	UInputAction* InteractAction;
};
