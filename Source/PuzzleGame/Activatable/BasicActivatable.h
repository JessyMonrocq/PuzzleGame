#pragma once

#include "CoreMinimal.h"
#include "Activatable.h"
#include "GameFramework/Actor.h"
#include "BasicActivatable.generated.h"

UCLASS()
class PUZZLEGAME_API ABasicActivatable : public AActor, public IActivatable
{
	GENERATED_BODY()
	
public:	
	ABasicActivatable();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetPower_Implementation(bool powerUp) override;
	virtual void SimpleInteract_Implementation() override;
};
