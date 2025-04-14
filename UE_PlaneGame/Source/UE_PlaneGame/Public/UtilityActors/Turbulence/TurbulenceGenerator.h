// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurbulenceGenerator.generated.h"

class UTurbulenceReciever;

UCLASS()
class UE_PLANEGAME_API ATurbulenceGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurbulenceGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector TurbulenceValue=FVector(0,0,0);
public:
	UFUNCTION(BlueprintCallable)
	FVector GetTurbulenceValue() const{return this->TurbulenceValue;}

	UFUNCTION(BlueprintCallable)
	void SetNoiseSpeed(float NewSpeed);

	UFUNCTION(BlueprintCallable)
	void SetTurbulenceStrength(float NewStrength);
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float NoiseSpeed=1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector TurbulenceModifier=FVector(0.3,1,0.5);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TurbulenceStrength=1;

private:
	UPROPERTY(Replicated)
	float NoiseDriver=0;


public:
	TArray<UTurbulenceReciever*> AllTurbulenceRecievers= TArray<UTurbulenceReciever*>();
	void SubscribeToTurbulence(UTurbulenceReciever* RecieverToAdd);

	
private:
	void BrodcastTurbulence();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive=false;
	
};
