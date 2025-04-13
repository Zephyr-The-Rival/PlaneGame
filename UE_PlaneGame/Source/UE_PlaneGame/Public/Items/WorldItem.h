// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UtilityActors/Turbulence/TurbulenceReciever.h"
#include "WorldItem.generated.h"

UCLASS()
class UE_PLANEGAME_API AWorldItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTurbulenceReciever* TurbulenceReciever;
};
