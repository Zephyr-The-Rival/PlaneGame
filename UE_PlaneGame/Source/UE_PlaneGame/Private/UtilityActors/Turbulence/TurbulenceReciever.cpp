// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityActors/Turbulence/TurbulenceReciever.h"

#include "ComponentUtils.h"
#include "Engine/TriggerBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/PlanePlayerCharacter.h"
#include "UtilityActors/Turbulence/TurbulenceGenerator.h"


// Sets default values for this component's properties
UTurbulenceReciever::UTurbulenceReciever()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTurbulenceReciever::BeginPlay()
{
	Super::BeginPlay();
	MyTurbulenceGenerator = Cast<ATurbulenceGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), ATurbulenceGenerator::StaticClass()));
	this->MyTurbulenceGenerator->SubscribeToTurbulence(this);

	GetOwner()->OnDestroyed.AddDynamic(this, &UTurbulenceReciever::OnParentDestroyed);
	
	// ...
	
}


// Called every frame
void UTurbulenceReciever::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTurbulenceReciever::ApplyTurbulence(float DeltaTime, const FVector& TurbulenceVector)
{
	if(!this->bActive)
		return;
	
	//apply turbulence gets executed on all clients but the server still has authority over the positions of items
	//however, before the netupdate comes, the items are moved on the client by the same noise value.
	//the noise is predetermined and the driving value is replicated.

	AActor* OwnerActor= GetOwner();
	
	if (APlanePlayerCharacter* PlayerCharacter = Cast<APlanePlayerCharacter>(OwnerActor))
	{
		//PlayerCharacter->AddActorWorldOffset(TurbulenceValue*DeltaTime);
		return;
	}

	
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OwnerActor->GetRootComponent()))
	{
		//PrimitiveComponent->AddForce(TurbulenceVector * DeltaTime * 100000);
		//PrimitiveComponent->AddVelocityChangeImpulseAtLocation(TurbulenceVector*DeltaTime, PrimitiveComponent->GetComponentLocation()*1000);
		//PrimitiveComponent->AddWorldOffset(TurbulenceVector*DeltaTime);
		PrimitiveComponent->AddForce(TurbulenceVector * DeltaTime * 5000 *PrimitiveComponent->GetMass());
	}
}

void UTurbulenceReciever::OnParentDestroyed(AActor* DestroyedActor)
{
	if(MyTurbulenceGenerator)
		MyTurbulenceGenerator->UnsubscribeFromTurbulence(this);
}




