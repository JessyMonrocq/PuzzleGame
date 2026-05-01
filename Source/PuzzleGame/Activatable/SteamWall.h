#pragma once

#include "CoreMinimal.h"
#include "Activatable.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "SteamWall.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class PUZZLEGAME_API ASteamWall : public AActor, public IActivatable
{
	GENERATED_BODY()
	
public:	
	ASteamWall();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> SteamWallCollider;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Wall References", meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<UNiagaraComponent>> SteamParticles;
	
	virtual void SetPower_Implementation(bool powerUp) override;
	virtual void Interact_Implementation(bool state) override;

private:
	void AttachSteamParticlesToRoot();
};
