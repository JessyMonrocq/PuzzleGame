#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "PuzzleGame/Activatable/Activatable.h"
#include "InteractableButton.generated.h"

class UStaticMeshComponent;
class UMaterialInstance;

UCLASS()
class PUZZLEGAME_API AInteractableButton : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractableButton();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void SetButtonState(bool isActive);

protected:
	enum class State
	{
		Active,
		Disabled,
		Busy,
	};
	
	State buttonState = State::Active;
	
	UPROPERTY(EditAnywhere)
	bool isDisabled = false;
	UPROPERTY(EditAnywhere)
	bool isOneTimeInteraction = false;
	UPROPERTY(EditAnywhere)
	bool isPushButton = false;
	
	bool wasInteractedWith = false;
	bool isPushed = false;
	
	FTimerHandle TimerHandle;
	float TickInterval = 0.016f;
	
	float elapsedTime = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float buttonAnimDuration = 0.12f;
	UPROPERTY(BlueprintReadWrite)
	float buttonPushDistance = 12.0f;
	bool isReturningFromPress = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Button References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> ButtonFrameMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Button References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> ButtonPlateMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Button References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMaterialInstance> HighlightMaterialInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Button References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMaterialInstance> DisabledMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<AActor> TargetActor;
	
	virtual bool IsHoldInteraction_Implementation() const override;
	virtual void Highlight_Implementation(bool detected) override;
	virtual void Interact_Implementation() override;
	
	void PressButton();
	void PushButton();
	bool AnimateButton(bool pressed, float animDuration);
};
