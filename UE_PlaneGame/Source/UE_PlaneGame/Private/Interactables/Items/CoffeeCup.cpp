// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Items/CoffeeCup.h"


ACoffeeCup::ACoffeeCup()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CupMesh"));
	RootComponent = CupMesh;
	
	CupMesh->SetSimulatePhysics(false);
	
	LiquidPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	LiquidPlane->SetupAttachment(CupMesh);
	
	LiquidDampingFactor = 0.95f;
	LiquidTiltResponse = 0.8f;
	LiquidSmoothness = 6.0f;
	MaxLiquidAngle = 25.0f;
	InertiaFactor = 0.2f;
	
	MotionResponseFactor = 1.5f;
	MotionDampingFactor = 0.9f;
	MotionSmoothness = 4.0f;
	MinSpeedThreshold = 5.0f;
	MaxMotionAngle = 15.0f;
	EnableMotionEffects = true;
	
	CurrentLiquidRotation = FVector::ZeroVector;
	TargetLiquidRotation = FVector::ZeroVector;
	PrevCupRotation = FVector::ZeroVector;
	
	PrevLocation = FVector::ZeroVector;
	CurrentVelocity = FVector::ZeroVector;
	PrevVelocity = FVector::ZeroVector;
	MotionOffset = FVector::ZeroVector;
	TargetMotionOffset = FVector::ZeroVector;
}

void ACoffeeCup::BeginPlay()
{
    Super::BeginPlay();
    

    InitializeLiquidPlane();
}


void ACoffeeCup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    

	CalculateLiquidRotation(DeltaTime);
    

	if (EnableMotionEffects)
	{
		CalculateMotionEffects(DeltaTime);
	}
    

	UpdateLiquidTransform();
}

void ACoffeeCup::InitializeLiquidPlane()
{
	if (LiquidPlane)
	{
		LiquidPlane->SetRelativeRotation(FRotator::ZeroRotator);
		
		CurrentLiquidRotation = FVector::ZeroVector;
		TargetLiquidRotation = FVector::ZeroVector;
		
		FRotator ActorRotation = GetActorRotation();
		PrevCupRotation = FVector(ActorRotation.Pitch, ActorRotation.Roll, ActorRotation.Yaw);

		PrevLocation = GetActorLocation();
		CurrentVelocity = FVector::ZeroVector;
		PrevVelocity = FVector::ZeroVector;
		MotionOffset = FVector::ZeroVector;
		TargetMotionOffset = FVector::ZeroVector;
	}
}


void ACoffeeCup::CalculateLiquidRotation(float DeltaTime)
{

	FRotator CupWorldRotation = GetActorRotation();
	FVector CurrentCupRotation = FVector(CupWorldRotation.Pitch, CupWorldRotation.Roll, CupWorldRotation.Yaw);
    

	FVector CupRotationDelta = CurrentCupRotation - PrevCupRotation;

	PrevCupRotation = CurrentCupRotation;
	
	float TargetX = FMath::Clamp(-CupWorldRotation.Pitch * LiquidTiltResponse, -MaxLiquidAngle, MaxLiquidAngle);
	float TargetY = FMath::Clamp(-CupWorldRotation.Roll * LiquidTiltResponse, -MaxLiquidAngle, MaxLiquidAngle);
	
	TargetX += -CupRotationDelta.X * InertiaFactor;
	TargetY += -CupRotationDelta.Y * InertiaFactor;
	
	TargetX = FMath::Clamp(TargetX, -MaxLiquidAngle, MaxLiquidAngle);
	TargetY = FMath::Clamp(TargetY, -MaxLiquidAngle, MaxLiquidAngle);
	
	TargetLiquidRotation = FVector(TargetX, TargetY, 0.0f);
	
	CurrentLiquidRotation = FMath::VInterpTo(CurrentLiquidRotation, TargetLiquidRotation, DeltaTime, LiquidSmoothness);
	
	float DampingRate = 1.0f - (LiquidDampingFactor * DeltaTime);
	TargetLiquidRotation *= DampingRate;
}

void ACoffeeCup::CalculateMotionEffects(float DeltaTime)
{

	FVector CurrentLocation = GetActorLocation();
	
	CurrentVelocity = (CurrentLocation - PrevLocation) / DeltaTime;
    

	FVector Acceleration = (CurrentVelocity - PrevVelocity) / DeltaTime;
	PrevLocation = CurrentLocation;
	PrevVelocity = CurrentVelocity;
	
	float AccelerationMagnitude = Acceleration.Size();
	if (AccelerationMagnitude > MinSpeedThreshold)
	{
		FVector AccelerationDirection = -Acceleration.GetSafeNormal();
		
		
		FRotator CupWorldRotation = GetActorRotation();

		FVector WorldRight = FRotationMatrix(CupWorldRotation).GetScaledAxis(EAxis::Y);
		FVector WorldForward = FRotationMatrix(CupWorldRotation).GetScaledAxis(EAxis::X);
		
		float ForwardAcceleration = FVector::DotProduct(AccelerationDirection, WorldForward);
		float RightAcceleration = FVector::DotProduct(AccelerationDirection, WorldRight);
		
		float MotionPitch = RightAcceleration * MotionResponseFactor; 
		float MotionRoll = -ForwardAcceleration * MotionResponseFactor; 

		MotionPitch = FMath::Clamp(MotionPitch, -MaxMotionAngle, MaxMotionAngle);
		MotionRoll = FMath::Clamp(MotionRoll, -MaxMotionAngle, MaxMotionAngle);
		
		TargetMotionOffset = FVector(MotionPitch, MotionRoll, 0.0f);
	}
	
	MotionOffset = FMath::VInterpTo(MotionOffset, TargetMotionOffset, DeltaTime, MotionSmoothness);
	
	float MotionDampingRate = 1.0f - (MotionDampingFactor * DeltaTime);
	TargetMotionOffset *= MotionDampingRate;
}

void ACoffeeCup::UpdateLiquidTransform()
{
	if (LiquidPlane)
	{
		FVector CombinedRotation = CurrentLiquidRotation;
        

		if (EnableMotionEffects)
		{
			CombinedRotation += MotionOffset;
			
			CombinedRotation.X = FMath::Clamp(CombinedRotation.X, -MaxLiquidAngle, MaxLiquidAngle);
			CombinedRotation.Y = FMath::Clamp(CombinedRotation.Y, -MaxLiquidAngle, MaxLiquidAngle);
		}
		
		FRotator NewRotation = FRotator(CombinedRotation.X, CombinedRotation.Y, 0.0f);
		
		LiquidPlane->SetRelativeRotation(NewRotation);
	}
}