// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/PlayerHand.h"
#include "Interactables/Interactable.h"
#include "Components/SphereComponent.h"
#include "Interactables/Items/WorldItem.h"


// Sets default values
APlayerHand::APlayerHand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	this->HandCollision= CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	this->HandCollision->SetupAttachment(this->RootComponent);
	
}

// Called when the game starts or when spawned
void APlayerHand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AInteractable* APlayerHand::GetOverlappingItem()
{
	TArray<AActor*> OverlappingActors;
	HandCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		AInteractable* Interactable = Cast<AInteractable>(Actor);
		if (Interactable)
		{
			return Interactable;
		}
	}
	return nullptr;
}

