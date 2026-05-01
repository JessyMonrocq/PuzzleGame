#include "PuzzleGame/Activatable/BasicActivatable.h"

ABasicActivatable::ABasicActivatable()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABasicActivatable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasicActivatable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicActivatable::SetPower_Implementation(bool powerUp)
{
	
}

void ABasicActivatable::SimpleInteract_Implementation()
{
	
}
