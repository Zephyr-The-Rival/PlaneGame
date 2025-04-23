// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Interactable.h"


// Sets default values
AInteractable::AInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::Interact_Implementation(APlanePlayerCharacter* InteractingPlayer)
{
	//OverrideInChildClasses
}

void AInteractable::HoldInteract_Start_Implementation(APlanePlayerCharacter* InteractingPlayer)
{
	//OverrideInChildClasses
}

void AInteractable::HoldInteract_End_Implementation(APlanePlayerCharacter* InteractingPlayer)
{
	//OverrideInChildClasses
}

