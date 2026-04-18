#include "PuzzleGame/Interactable/InteractableComponent.h"
#include "Components/BoxComponent.h"

AInteractableComponent::AInteractableComponent()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	SetRootComponent(InteractionBox);

	InteractionBox->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	//InteractionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableComponent::Interact(bool State)
{
	if (!canInteract)
	{
		return;
	}

	if (isHoldInteraction)
	{
		HoldInteract(State);
	}

	if (State)
	{
		SimpleInteract();
	}
}

void AInteractableComponent::SimpleInteract_Implementation()
{
	isInteracted = !isInteracted;
}

void AInteractableComponent::HoldInteract_Implementation(bool State)
{
	isInteracted = State;
}

