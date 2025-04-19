// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Interactables/Interactable.h"
#include "GrabHandle.generated.h"

class APlayerHand;
class APlanePlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGrabEvent, APlayerHand*, Hand, APlanePlayerCharacter*, PlayerCharacter);


UCLASS()
class UE_PLANEGAME_API AGrabHandle : public AInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrabHandle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FGrabEvent OnGrabbed;
	
	UPROPERTY(BlueprintAssignable)
	FGrabEvent OnLetGo;
};
