#include "PuzzleGame/Interactable/InteractableValve.h"
#include "Components/StaticMeshComponent.h"

AInteractableValve::AInteractableValve()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableValve::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableValve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AInteractableValve::IsHoldInteraction_Implementation() const
{
	return true;
}

void AInteractableValve::Highlight_Implementation(bool detected)
{
	if (!detected || valveState == State::Busy)
	{
		ValveWheelMesh->SetOverlayMaterial(nullptr);
		return;
	}

	if (wasInteractedWith)
	{
		return;
	}
	else
	{
		if (valveState == State::Active)
		{
			ValveWheelMesh->SetOverlayMaterial(HighlightMaterialInstance);
		}
	}
}

void AInteractableValve::HoldInteraction_Implementation(bool interact)
{
	if (valveState == State::Busy || wasInteractedWith)
	{
		return;
	}

	ValveWheelMesh->SetOverlayMaterial(nullptr);
	TickInterval = GetWorld()->GetDeltaSeconds();

	if (interact)
	{
		elapsedTime = 0.0f;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&AInteractableValve::RotateValve,
			TickInterval,
			true
		);
	}
	else
	{
		valveState = State::Busy;
		resetDuration = elapsedTime;
		elapsedTime = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&AInteractableValve::CloseValve,
			TickInterval,
			true
		);
	}
}

void AInteractableValve::RotateValve()
{
	if (AnimateValve(true))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		wasInteractedWith = true;
		ValveWheelMesh->SetOverlayMaterial(nullptr);
		
		if (IActivatable* Activatable = Cast<IActivatable>(TargetActor))
		{
			Activatable->Execute_SimpleInteract(TargetActor);
		}
	}
}

void AInteractableValve::CloseValve()
{
	if (AnimateValve(false))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		valveState = State::Active;
	}
}

bool AInteractableValve::AnimateValve(bool rotate)
{	
	float start = rotate ? 0.0f : ValveWheelMesh->GetRelativeRotation().Roll;
	float end = rotate ? maxRotationAngle : 0.0f;
	
	elapsedTime += TickInterval;
	float duration = rotate ? animDuration : resetDuration;
	float Alpha = FMath::Clamp(elapsedTime / duration, 0.0f, 1.0f);
	
	float XOffset = FMath::Lerp(start, end, Alpha);
	FRotator rotator = ValveWheelMesh->GetRelativeRotation();
	rotator.Roll = XOffset;
	
	ValveWheelMesh->SetRelativeRotationExact(rotator);
	
	return elapsedTime >= duration;
}
