// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Tool.h"

ATool::ATool(): LastMovementDirection()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATool::BeginPlay()
{
    Super::BeginPlay();
    
    bToolActive = false;
    
}

void ATool::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void ATool::OnHandMovement(FVector MovementVector)
{
    UE_LOG(LogTemp, Log, TEXT("ATool::OnHandMovement - Movements are detected and the vector: X=%f, Y=%f, Z=%f"), 
           MovementVector.X, MovementVector.Y, MovementVector.Z);
    
    if (!bToolActive)
    {
       bToolActive = true;
       UE_LOG(LogTemp, Warning, TEXT("ATool::OnHandMovement - Tool activate"));
       ActivateTool(MovementVector);
    }
    
    LastMovementDirection = MovementVector.GetSafeNormal();
    
    UE_LOG(LogTemp, Verbose, TEXT("ATool::OnHandMovement - Normalize the direction of movement: X=%f, Y=%f, Z=%f"), 
           LastMovementDirection.X, LastMovementDirection.Y, LastMovementDirection.Z);
    
}

void ATool::OnHandMovementStopped()
{
    
    if (bToolActive)
    {
       bToolActive = false;
       UE_LOG(LogTemp, Warning, TEXT("ATool::OnHandMovementStopped - Tool deactivate"));
       DeactivateTool();
    }
}

void ATool::ActivateTool_Implementation(FVector MovementVector)
{
    
}

void ATool::DeactivateTool_Implementation()
{
    
}