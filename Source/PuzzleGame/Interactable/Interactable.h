#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class PUZZLEGAME_API IInteractable
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsHoldInteraction() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Highlight(bool detected);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HoldInteraction(bool interacting);
};
