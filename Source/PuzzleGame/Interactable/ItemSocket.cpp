#include "PuzzleGame/Interactable/ItemSocket.h"

#include "PuzzleGame/Player/PlayerCharacter.h"

AItemSocket::AItemSocket()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItemSocket::BeginPlay()
{
	Super::BeginPlay();
	
	isSocketed = false;
	SocketedItem = nullptr;
}

void AItemSocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemSocket::Highlight_Implementation(bool detected)
{
	if (detected)
	{
		if (isSocketed)
		{
			SocketMesh->SetOverlayMaterial(HighlightMaterialInstance);
			return;
		}
		
		TObjectPtr<APlayerCharacter> PlayerCharacter = APlayerCharacter::Get(this);
		if (PlayerCharacter.IsNull() || PlayerCharacter->GetPickupItem() == nullptr)
		{
			return;
		}
		
		if (PlayerCharacter->GetPickupItem()->GetItemKey() == ItemKey)
		{
			SocketMesh->SetOverlayMaterial(HighlightMaterialInstance);
			correctKeyDetected = true;
		}
	}
	else
	{
		SocketMesh->SetOverlayMaterial(nullptr);
		correctKeyDetected = false;
	}
}

void AItemSocket::Interact_Implementation()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = APlayerCharacter::Get(this);
	if (!isSocketed && !PlayerCharacter.IsNull() && correctKeyDetected)
	{
		SocketedItem = PlayerCharacter->GetPickupItem();
		SocketedItem->SetItemPickupState(false);
		PlayerCharacter->SocketItem(SocketPoint);
		
		isSocketed = true;
	} else if (isSocketed && !PlayerCharacter.IsNull() && PlayerCharacter->GetPickupItem() == nullptr)
	{
		SocketedItem->SetItemPickupState(true);
		PlayerCharacter->PickupItem(SocketedItem);
		SocketedItem = nullptr;
		
		isSocketed = false;
	}
}
