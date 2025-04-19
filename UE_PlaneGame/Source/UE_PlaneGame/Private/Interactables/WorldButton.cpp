// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/WorldButton.h"

#include "Components/BoxComponent.h"


// Sets default values
AWorldButton::AWorldButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->ButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Base"));
	this->ButtonBase->SetupAttachment(RootComponent);

	this->Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	this->Button->SetupAttachment(this->ButtonBase);

	this->InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Collision"));
	this->InteractCollision->SetupAttachment(this->ButtonBase);
	
}

// Called when the game starts or when spawned
void AWorldButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldButton::Press_Implementation(APlanePlayerCharacter* Player)
{
	//will be defined in blueprints;
}

