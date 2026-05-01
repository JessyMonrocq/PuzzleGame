#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Activatable.generated.h"

UINTERFACE(MinimalAPI)
class UActivatable : public UInterface
{
	GENERATED_BODY()
};

class PUZZLEGAME_API IActivatable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPower(bool powerUp);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SimpleInteract();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(bool state);
};
