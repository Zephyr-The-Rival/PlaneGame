// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/WorldItem.h"
#include "GrabHandle.generated.h"

UCLASS()
class UE_PLANEGAME_API AGrabHandle : public AWorldItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrabHandle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
