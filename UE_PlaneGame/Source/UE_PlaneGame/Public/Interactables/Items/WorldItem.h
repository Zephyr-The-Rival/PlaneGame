// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/Interactable.h"
#include "UtilityActors/Turbulence/TurbulenceReciever.h"
#include "WorldItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleEvent);

UCLASS()
class UE_PLANEGAME_API AWorldItem : public AInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTurbulenceReciever* TurbulenceReciever;

	UPROPERTY(BlueprintAssignable)
	FSimpleEvent OnPickedUp_Server;
	
	UPROPERTY(BlueprintAssignable)
	FSimpleEvent OnDropped_Server;
private:

	UFUNCTION(NetMulticast, Reliable)
	void MC_OnPickedUp();
	void MC_OnPickedUp_Implementation();
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnDropped();
	void MC_OnDropped_Implementation();

private:
	void CheckUp();

protected:
	
	virtual void Interact_Implementation(APlanePlayerCharacter* InteractingPlayer) override;
};
