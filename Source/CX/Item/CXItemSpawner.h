#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CXItemSpawner.generated.h"

class UBillboardComponent;

UCLASS()
class CX_API ACXItemSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACXItemSpawner();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void SpawnItem();

    UFUNCTION()
    void OnItemDestroyed(AActor* DestroyedActor);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    TSubclassOf<AActor> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float SpawnInterval = 2.f;

    UPROPERTY()
    TObjectPtr<AActor> SpawnedItem;

    FTimerHandle SpawnTimerHandle;

protected:
    UPROPERTY()
    TObjectPtr<UBillboardComponent> BillboardComponent;
};