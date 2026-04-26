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
	bool isOpen = false;
	
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
	
	virtual void SimpleActivate_Implementation() override;
	virtual void Activate_Implementation(bool state) override;
	
	void AnimateDoor();
};
