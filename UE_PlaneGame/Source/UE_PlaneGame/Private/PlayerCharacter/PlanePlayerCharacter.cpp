// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_PlaneGame/Public/PlayerCharacter/PlanePlayerCharacter.h"

#include "Camera/CameraComponent.h"


// Sets default values
APlanePlayerCharacter::APlanePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	FirstPersonMesh->SetupAttachment(RootComponent);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void APlanePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlanePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlanePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

