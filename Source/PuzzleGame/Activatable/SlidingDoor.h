#pragma once

#include "CoreMinimal.h"
#include "Activatable.h"
#include "GameFramework/Actor.h"
#include "SlidingDoor.generated.h"

UCLASS()
class PUZZLEGAME_API ASlidingDoor : public AActor, public IActivatable
{
	GENERATED_BODY()
	
public:	
	ASlidingDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activatable")
	bool IsPoweredOnStart = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activatable")
	bool InteractOnPowered = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activatable")
	int requiredPower;
	
	bool isPowered;
	int currentPower;
	
	bool isOpen;
	
	FTimerHandle TimerHandle;
	float TickInterval = 0.016f;
	
	float elapsedTime = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float animDuration = 0.2f;
	UPROPERTY(BlueprintReadWrite)
	float doorOpeningDistance = 25.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> DoorLeftMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> DoorRightMesh;
	
	virtual void SetPower_Implementation(bool powerUp) override;
	virtual void SimpleInteract_Implementation() override;
	
	void AnimateDoor();
};
