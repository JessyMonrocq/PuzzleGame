#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "PuzzleGame/Activatable/Activatable.h"
#include "InteractableValve.generated.h"

class UStaticMeshComponent;
class UMaterialInstance;

UCLASS()
class PUZZLEGAME_API AInteractableValve : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractableValve();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
protected:
	enum class State
	{
		Active,
		Busy,
	};
	
	State valveState = State::Active;
	
	bool wasInteractedWith = false;
	
	FTimerHandle TimerHandle;
	float TickInterval = 0.016f;
	
	float elapsedTime = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float animDuration = 2.0f;
	float resetDuration = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float maxRotationAngle = 360.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Valve References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> ValveWheelMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Valve References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMaterialInstance> HighlightMaterialInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Valve References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMaterialInstance> DisabledMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<AActor> TargetActor;
	
	virtual bool IsHoldInteraction_Implementation() const override;
	virtual void Highlight_Implementation(bool detected) override;
	virtual void HoldInteraction_Implementation(bool interact) override;
	
	void RotateValve();
	void CloseValve();
	bool AnimateValve(bool rotate);
};
