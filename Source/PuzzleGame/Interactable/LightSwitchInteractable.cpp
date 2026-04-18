#include "PuzzleGame/Interactable/LightSwitchInteractable.h"

#include "Components/PointLightComponent.h"

ALightSwitchInteractable::ALightSwitchInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	LightComponent->SetupAttachment(RootComponent);
}

void ALightSwitchInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	if (LightComponent)
	{
		LightComponent->SetIntensity(isInteracted ? lightIntensity : 0.0f);
	}
}

void ALightSwitchInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightSwitchInteractable::SimpleInteract_Implementation()
{
	Super::SimpleInteract_Implementation();

	if (LightComponent)
	{
		LightComponent->SetIntensity(isInteracted ? lightIntensity : 0.0f);
	}
}
