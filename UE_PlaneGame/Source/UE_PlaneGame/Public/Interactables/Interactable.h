// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class APlanePlayerCharacter;

UCLASS()
class UE_PLANEGAME_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION(BlueprintNativeEvent)
	void Interact(APlanePlayerCharacter* InteractingPlayer);
	virtual void Interact_Implementation(APlanePlayerCharacter* InteractingPlayer);

	UFUNCTION(BlueprintNativeEvent)
	void HoldInteract_Start(APlanePlayerCharacter* InteractingPlayer);
	virtual void HoldInteract_Start_Implementation(APlanePlayerCharacter* InteractingPlayer);

	UFUNCTION(BlueprintNativeEvent)
	void HoldInteract_End(APlanePlayerCharacter* InteractingPlayer);
	virtual void HoldInteract_End_Implementation(APlanePlayerCharacter* InteractingPlayer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHoldToInteract=false;
	
};
