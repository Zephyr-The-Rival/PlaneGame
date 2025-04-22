// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Lever.h"


// Sets default values
ALever::ALever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Base=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever Base"));
	this->Base->SetupAttachment(this->RootComponent);

	this->Handle=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever Handle"));
	this->Handle->SetupAttachment(this->Base);
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

