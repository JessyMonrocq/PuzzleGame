#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

class UStaticMeshComponent;

UCLASS()
class PUZZLEGAME_API APickupItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	APickupItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void SetItemPhysics(bool state);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item References")
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMaterialInstance> HighlightMaterialInstance;
	
	virtual void Highlight_Implementation(bool detected) override;
	virtual void Interact_Implementation() override;
	
	
};
