// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/PlayerHand.h"

#include "Debug.h"
#include "Camera/CameraComponent.h"
#include "Interactables/Interactable.h"
#include "Components/SphereComponent.h"
#include "Interactables/Items/WorldItem.h"
#include "PlayerCharacter/PlanePlayerCharacter.h"


// Sets default values
APlayerHand::APlayerHand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Target = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	this->Target->SetupAttachment(this->GetRootComponent());
	
	this->HandCollision= CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	this->HandCollision->SetupAttachment(this->Target);
	
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
	Tick_SetCollisionPosition();
}

AInteractable* APlayerHand::GetOverlappingInteractable()
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

void APlayerHand::Tick_SetCollisionPosition()
{
	if(!MyOwningPlayer)
	{
		Debug::Print("PlayerHand.MyOwningPlayer is null",GetWorld()->GetDeltaSeconds(),FColor::Red);
		return;
	}
	

	FVector Start= MyOwningPlayer->GetCamera()->GetComponentLocation();
	FVector End= this->Target->GetComponentLocation();

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, GetOwner());
	
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 1.0f);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (bHit)
		this->HandCollision->SetWorldLocation(HitResult.Location);
	else
		this->HandCollision->SetWorldLocation(HitResult.TraceEnd);
}

