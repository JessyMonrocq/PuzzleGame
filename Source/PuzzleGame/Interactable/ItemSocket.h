#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "PuzzleGame/Interactable/PickupItem.h"
#include "PuzzleGame/DataAssets/ItemKey.h"
#include "ItemSocket.generated.h"


UCLASS()
class PUZZLEGAME_API AItemSocket : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AItemSocket();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	
	bool isSocketed = false;
	bool correctKeyDetected = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Socket References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMaterialInstance> HighlightMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> SocketMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Socket References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> SocketPoint;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APickupItem> SocketedItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Socket References", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UItemKey> ItemKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activatable")
	TObjectPtr<AActor> TargetActor;
	
	virtual void Highlight_Implementation(bool detected) override;
	virtual void Interact_Implementation() override;
};
