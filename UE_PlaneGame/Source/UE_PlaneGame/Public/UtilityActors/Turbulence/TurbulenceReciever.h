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
private:
	//is set on begin play
	ATurbulenceGenerator* MyTurbulenceGenerator;

public:
	void ApplyTurbulence(float DeltaTime, const FVector& TurbulenceVector);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActive=true;

private:
	//add recieving components to this list if root isnt only recieving component
	TArray<UPrimitiveComponent*> RecievingComponents =TArray<UPrimitiveComponent*>();
public:

	UFUNCTION(BlueprintCallable)
	void AddRecievingComponent(UPrimitiveComponent* PrimitiveComponent);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StrengthMultiplier=1;

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UFUNCTION()
	void OnParentDestroyed(AActor* DestroyedActor);

};
