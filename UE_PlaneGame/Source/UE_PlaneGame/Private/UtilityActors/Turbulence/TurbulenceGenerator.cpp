// Fill out your copyright notice in the Description page of Project Settings.



#include "UtilityActors/Turbulence//TurbulenceGenerator.h"
#include "Debug.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter/PlanePlayerCharacter.h"
 #include "UtilityActors/Turbulence/TurbulenceReciever.h"


// Sets default values
ATurbulenceGenerator::ATurbulenceGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates=true;
}

// Called when the game starts or when spawned
void ATurbulenceGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurbulenceGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
		this->NoiseDriver += DeltaTime * NoiseSpeed;

	FVector TargetValue = FVector(FMath::PerlinNoise1D(NoiseDriver / 3), FMath::PerlinNoise1D(NoiseDriver),
	                              FMath::PerlinNoise1D(NoiseDriver / 2));
	TargetValue *= TurbulenceModifier;
	TargetValue *= TurbulenceStrength;

	TurbulenceValue = TargetValue;

	BrodcastTurbulence();
}

void ATurbulenceGenerator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATurbulenceGenerator, NoiseDriver)
}

void ATurbulenceGenerator::SetNoiseSpeed(const float NewSpeed)
{
	this->NoiseSpeed=NewSpeed;
}

void ATurbulenceGenerator::SetTurbulenceStrength(const float NewStrength)
{
	this->TurbulenceStrength=NewStrength;
}

void ATurbulenceGenerator::BrodcastTurbulence()
{
	for(UTurbulenceReciever* Reciever:this->AllTurbulenceRecievers)
	{
		Reciever->ApplyTurbulence(GetWorld()->GetDeltaSeconds(), this->TurbulenceValue);
	}
}

void ATurbulenceGenerator::SubscribeToTurbulence(UTurbulenceReciever* RecieverToAdd)
{
	this->AllTurbulenceRecievers.Add(RecieverToAdd);
}


