#include "PuzzleGame/Interactable/ItemSocket.h"

#include "PuzzleGame/Activatable/Activatable.h"
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
	TObjectPtr<APlayerCharacter> PlayerCharacter = APlayerCharacter::Get(this);
	
	if (detected)
	{
		if (isSocketed)
		{
			SocketMesh->SetOverlayMaterial(HighlightMaterialInstance);
			FText displayText = FText::FromString("Retrieve 'E'");
			PlayerCharacter->SetInteractWidgetText(displayText);
			PlayerCharacter->DisplayInteractWidget(true);
			return;
		}

		if (PlayerCharacter.IsNull() || PlayerCharacter->GetPickupItem() == nullptr)
		{
			return;
		}

		if (PlayerCharacter->GetPickupItem()->GetItemKey() == ItemKey)
		{
			SocketMesh->SetOverlayMaterial(HighlightMaterialInstance);
			FText displayText = FText::FromString("Insert 'E'");
			PlayerCharacter->SetInteractWidgetText(displayText);
			PlayerCharacter->DisplayInteractWidget(true);
			correctKeyDetected = true;
		}
	}
	else
	{
		SocketMesh->SetOverlayMaterial(nullptr);
		PlayerCharacter->DisplayInteractWidget(false);
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
		Highlight_Implementation(true);
		
		if (IActivatable* Activatable = Cast<IActivatable>(TargetActor))
		{
			Activatable->Execute_SetPower(TargetActor, true);
		}
	}
	else if (isSocketed && !PlayerCharacter.IsNull() && PlayerCharacter->GetPickupItem() == nullptr)
	{
		SocketedItem->SetItemPickupState(true);
		PlayerCharacter->PickupItem(SocketedItem);
		SocketedItem = nullptr;
		isSocketed = false;
		Highlight_Implementation(true);
		
		if (IActivatable* Activatable = Cast<IActivatable>(TargetActor))
		{
			Activatable->Execute_SetPower(TargetActor, false);
		}
	}
}
