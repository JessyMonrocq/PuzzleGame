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

void ABasicActivatable::SimpleActivate_Implementation()
{
	
}

void ABasicActivatable::Activate_Implementation(bool state)
{
	
}
