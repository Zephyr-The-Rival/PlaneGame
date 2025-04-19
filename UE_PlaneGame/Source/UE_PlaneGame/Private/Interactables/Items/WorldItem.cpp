// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Items/WorldItem.h"

#include "Debug.h"
#include "UtilityActors/Turbulence/TurbulenceReciever.h"

// Sets default values
AWorldItem::AWorldItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TurbulenceReciever=CreateDefaultSubobject<UTurbulenceReciever>(TEXT("Turbulence Reciever "));
}

// Called when the game starts or when spawned
void AWorldItem::BeginPlay()
{
	Super::BeginPlay();
	
	CheckUp();
	OnPickedUp_Server.AddDynamic(this, &AWorldItem::MC_OnPickedUp);
	OnDropped_Server.AddDynamic(this,&AWorldItem::MC_OnDropped);
}

// Called every frame
void AWorldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldItem::MC_OnPickedUp_Implementation()
{
	if(UPrimitiveComponent* PhysicsComponent=Cast<UPrimitiveComponent>(this->GetRootComponent()))
	{
		PhysicsComponent->SetSimulatePhysics(false);
		PhysicsComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
	
	if(this->TurbulenceReciever)
	{
		this->TurbulenceReciever->bActive=false;
	}
}

void AWorldItem::MC_OnDropped_Implementation()
{
	if(UPrimitiveComponent* PhysicsComponent=Cast<UPrimitiveComponent>(this->GetRootComponent()))
	{
		PhysicsComponent->SetSimulatePhysics(true);
		PhysicsComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
	if(this->TurbulenceReciever)
	{
		this->TurbulenceReciever->bActive=true;
	}
}

void AWorldItem::CheckUp()
{
	if(!this->bReplicates)
		Debug::Print(this->GetName()+ ": bReplicates is false",10,FColor::Red);

	UPrimitiveComponent* PrimitiveRootComponent=Cast<UPrimitiveComponent>(GetRootComponent());
	if(!PrimitiveRootComponent)
	{
		Debug::Print(this->GetName()+ ": Root component is not a primitive component",10,FColor::Red);
		return;
	}
	
	if(!PrimitiveRootComponent->GetIsReplicated())
	{
		Debug::Print(this->GetName()+ ": Root component does not replicate",10,FColor::Red);
		Debug::Print(this->GetName()+ ": Also Check if ReplicatesMovement should be active!",10,FColor::Red);
		return;
	}
	
}

