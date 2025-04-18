// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurbulenceReciever.generated.h"


class ATurbulenceGenerator;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_PLANEGAME_API UTurbulenceReciever : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	
	UTurbulenceReciever();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ATurbulenceGenerator* MyTurbulenceGenerator;
	
	UFUNCTION(Blueprintable)
	void ApplyTurbulence(float DeltaTime, const FVector& TurbulenceVector);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive=true;

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
