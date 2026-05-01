#include "PuzzleGame/Activatable/SlidingDoor.h"

ASlidingDoor::ASlidingDoor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	FVector locationLeftDoor = DoorLeftMesh->GetRelativeLocation();
	FVector locationRightDoor = DoorRightMesh->GetRelativeLocation();
	locationLeftDoor.X = isOpen ? -doorOpeningDistance : 0.0f;
	locationRightDoor.X = isOpen ? doorOpeningDistance : 0.0f;
	DoorLeftMesh->SetRelativeLocation(locationLeftDoor);
	DoorRightMesh->SetRelativeLocation(locationRightDoor);

	if (IsPoweredOnStart)
	{
		currentPower = requiredPower;
		isPowered = true;
		if (InteractOnPowered)
		{
			SimpleInteract_Implementation();
		}
	}
	else
	{
		currentPower = 0;
		isPowered = false;
	}
}

void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlidingDoor::SetPower_Implementation(bool powerUp)
{
	currentPower += powerUp ? 1 : -1;
	if (currentPower > requiredPower)
	{
		currentPower = requiredPower;
	}
	else if (currentPower < 0)
	{
		currentPower = 0;
	}

	if (currentPower == requiredPower)
	{
		isPowered = true;
		if (InteractOnPowered)
		{
			SimpleInteract_Implementation();
		}
	}
	else if (currentPower < requiredPower && isPowered)
	{
		if (isOpen)
		{
			SimpleInteract_Implementation();
		}
		isPowered = false;
	}
}

void ASlidingDoor::SimpleInteract_Implementation()
{
	if (!isPowered)
	{
		return;
	}

	isOpen = !isOpen;

	elapsedTime = 0.0f;
	TickInterval = GetWorld()->GetDeltaSeconds();

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ASlidingDoor::AnimateDoor,
		TickInterval,
		true
	);
}

void ASlidingDoor::AnimateDoor()
{
	float start = isOpen ? 0.0f : doorOpeningDistance;
	float end = isOpen ? doorOpeningDistance : 0.0f;

	elapsedTime += TickInterval;
	float Alpha = FMath::Clamp(elapsedTime / animDuration, 0.0f, 1.0f);

	float XOffsetLeftDoor = FMath::Lerp(-start, -end, Alpha);
	float XOffsetRightDoor = FMath::Lerp(start, end, Alpha);
	FVector locationLeftDoor = DoorLeftMesh->GetRelativeLocation();
	FVector locationRightDoor = DoorRightMesh->GetRelativeLocation();
	locationLeftDoor.X = XOffsetLeftDoor;
	locationRightDoor.X = XOffsetRightDoor;

	DoorRightMesh->SetRelativeLocation(locationRightDoor);
	DoorLeftMesh->SetRelativeLocation(locationLeftDoor);

	if (elapsedTime >= animDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
