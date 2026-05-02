#include "PuzzleGame/Interactable/PickupItem.h"

#include "Components/StaticMeshComponent.h"
#include "PuzzleGame/Player/PlayerCharacter.h"

APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetMobility(EComponentMobility::Movable);
	ItemMesh->SetSimulatePhysics(true);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();
}

void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItem::SetItemPhysics(bool state)
{
	ItemMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	ItemMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	ItemMesh->SetSimulatePhysics(state);
	ItemMesh->SetEnableGravity(state);
	ItemMesh->SetCollisionEnabled(state ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void APickupItem::SetItemPickupState(bool state)
{
	canPickup = state;
}

TObjectPtr<UItemKey> APickupItem::GetItemKey() const
{
	if (ItemKey != nullptr)
	{
		return ItemKey;
	}
	return nullptr;
}

void APickupItem::Highlight_Implementation(bool detected)
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = APlayerCharacter::Get(this);
	FText displayText = FText::FromString("Grab 'E'");
	PlayerCharacter->SetInteractWidgetText(displayText);
	
	if ((!PlayerCharacter.IsNull() && !PlayerCharacter->CanPickupItem()) || !canPickup)
	{
		return;
	}
	
	if (!detected)
	{
		ItemMesh->SetOverlayMaterial(nullptr);
		PlayerCharacter->DisplayInteractWidget(false);
	}
	else
	{
		ItemMesh->SetOverlayMaterial(HighlightMaterialInstance);
		PlayerCharacter->DisplayInteractWidget(true);
	}
}

void APickupItem::Interact_Implementation()
{
	TObjectPtr<APlayerCharacter> PlayerCharacter = APlayerCharacter::Get(this);
	if (!PlayerCharacter.IsNull() && PlayerCharacter->CanPickupItem())
	{
		PlayerCharacter->PickupItem(this);
		ItemMesh->SetOverlayMaterial(nullptr);
		PlayerCharacter->DisplayInteractWidget(false);
	}
}

