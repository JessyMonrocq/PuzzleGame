#pragma once

#include "PuzzleGame/Interactable/InteractableComponent.h"
#include "LightSwitchInteractable.generated.h"

class UPointLightComponent;

UCLASS()
class PUZZLEGAME_API ALightSwitchInteractable : public AInteractableComponent
{
	GENERATED_BODY()
	
public:	
	ALightSwitchInteractable();

protected:
	virtual void BeginPlay() override;
	virtual void SimpleInteract_Implementation() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Light", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPointLightComponent> LightComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Light", meta=(AllowPrivateAccess="true"))
	float lightIntensity = 5000.0f;
};
