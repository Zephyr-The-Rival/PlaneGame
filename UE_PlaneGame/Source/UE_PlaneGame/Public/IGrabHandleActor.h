// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGrabHandleActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIGrabHandleActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_PLANEGAME_API IIGrabHandleActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hello")
	void OnHandGrabbed(APlayerHand* Hand, APlanePlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hello")
	void OnHandReleased(APlayerHand* Hand, APlanePlayerCharacter* PlayerCharacter);
};
