
#include "Items/WobbleWater.h"


AWobbleWater::AWobbleWater()
{

    PrimaryActorTick.bCanEverTick = true;


    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}


void AWobbleWater::BeginPlay()
{
    Super::BeginPlay();
    
    if (MeshComponent->GetMaterial(0))
    {
        DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
    }
    
    LastPosition = GetActorLocation();
    LastRotation = GetActorRotation();
}

void AWobbleWater::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!DynamicMaterial)
        return;
    
    Time += DeltaTime;
    
    WobbleAmountToAddX = FMath::Lerp(WobbleAmountToAddX, 0.0f, DeltaTime * Recovery);
    WobbleAmountToAddZ = FMath::Lerp(WobbleAmountToAddZ, 0.0f, DeltaTime * Recovery);
    
    Pulse = 2 * PI * WobbleSpeed;
    WobbleAmountX = WobbleAmountToAddX * FMath::Sin(Pulse * Time);
    WobbleAmountZ = WobbleAmountToAddZ * FMath::Sin(Pulse * Time);
    
    DynamicMaterial->SetScalarParameterValue(FName("WobbleX"), WobbleAmountX);
    DynamicMaterial->SetScalarParameterValue(FName("WobbleZ"), WobbleAmountZ);
    
    FVector CurrentPosition = GetActorLocation();
    Velocity = (CurrentPosition - LastPosition) / DeltaTime;
    
    FRotator CurrentRotation = GetActorRotation();
    AngularVelocity = FVector(
        FMath::UnwindDegrees(CurrentRotation.Pitch - LastRotation.Pitch),
        FMath::UnwindDegrees(CurrentRotation.Yaw - LastRotation.Yaw),
        FMath::UnwindDegrees(CurrentRotation.Roll - LastRotation.Roll)
    );

    WobbleAmountToAddX += FMath::Clamp((Velocity.X + (AngularVelocity.Z * 0.2f)) * MaxWobble, -MaxWobble, MaxWobble);
    WobbleAmountToAddZ += FMath::Clamp((Velocity.Z + (AngularVelocity.X * 0.2f)) * MaxWobble, -MaxWobble, MaxWobble);
    
    LastPosition = CurrentPosition;
    LastRotation = CurrentRotation;
}