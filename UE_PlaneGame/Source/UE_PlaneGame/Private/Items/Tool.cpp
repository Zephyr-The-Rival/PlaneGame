// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Tool.h"

ATool::ATool(): LastMovementDirection()
{
    PrimaryActorTick.bCanEverTick = true;
    

    MovementThreshold = 100.0f;
    AccumulatedDistance = 0.0f;
    LastPosition = FVector::ZeroVector;
}

void ATool::BeginPlay()
{
    Super::BeginPlay();
    
    bToolActive = false;
    AccumulatedDistance = 0.0f;
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
       LastPosition = MovementVector; 
       AccumulatedDistance = 0.0f;  
       ActivateTool(MovementVector);
    }
    else
    {
        float MoveDist = FVector::Dist(MovementVector, LastPosition);
        AccumulatedDistance += MoveDist;
        
        UE_LOG(LogTemp, Verbose, TEXT("ATool::OnHandMovement - Movement distance: %f, Accumulated: %f, Threshold: %f"), 
               MoveDist, AccumulatedDistance, MovementThreshold);
        
        if (AccumulatedDistance >= MovementThreshold)
        {
            UE_LOG(LogTemp, Warning, TEXT("ATool::OnHandMovement - Movement threshold reached: %f"), AccumulatedDistance);
            OnMovementThresholdReached();
            AccumulatedDistance = 0.0f; 
        }
        
        LastPosition = MovementVector;
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
        
       AccumulatedDistance = 0.0f;
    }
}

void ATool::ActivateTool_Implementation(FVector MovementVector)
{
}

void ATool::DeactivateTool_Implementation()
{
}

void ATool::OnMovementThresholdReached_Implementation()
{
}