#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableComponent.generated.h"

class UBoxComponent;

UCLASS()
class PUZZLEGAME_API AInteractableComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableComponent();

	UFUNCTION(BlueprintCallable, Category="Interaction")
	void Interact(bool State);

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	void SimpleInteract();
	virtual void SimpleInteract_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	void HoldInteract(bool State);
	virtual void HoldInteract_Implementation(bool State);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool canInteract = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Interaction")
	bool isInteracted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool isHoldInteraction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TObjectPtr<UBoxComponent> InteractionBox;

public:	
	virtual void Tick(float DeltaTime) override;

};
