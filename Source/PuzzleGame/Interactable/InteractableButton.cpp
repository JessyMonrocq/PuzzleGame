#include "PuzzleGame/Interactable/InteractableButton.h"

#include <string>

#include "Components/StaticMeshComponent.h"

AInteractableButton::AInteractableButton()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractableButton::BeginPlay()
{
	Super::BeginPlay();
	
	SetButtonState(!isDisabled);
}

void AInteractableButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableButton::SetButtonState(bool isActive)
{
	buttonState = isActive ? State::Active : State::Disabled;
}

void AInteractableButton::Highlight_Implementation(bool detected)
{
	if (!detected)
	{
		ButtonFrameMesh->SetOverlayMaterial(nullptr);
		return;
	}

	if (wasInteractedWith && isOneTimeInteraction)
	{
		return;
	}
	else
	{
		if (buttonState == State::Active)
		{
			ButtonFrameMesh->SetOverlayMaterial(HighlightMaterialInstance);
		}
		else if (buttonState == State::Disabled)
		{
			ButtonFrameMesh->SetOverlayMaterial(DisabledMaterialInstance);
		}
	}
}

void AInteractableButton::Interact_Implementation()
{
	if (buttonState != State::Active || wasInteractedWith)
	{
		return;
	}
	
	ButtonFrameMesh->SetOverlayMaterial(nullptr);

	buttonState = State::Busy;
	elapsedTime = 0.0f;
	isReturningFromPress = false;
	TickInterval = GetWorld()->GetDeltaSeconds();
	if (isPushButton)
	{
		isPushed = !isPushed;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&AInteractableButton::PushButton,
			TickInterval,
			true
		);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&AInteractableButton::PressButton,
			TickInterval,
			true
		);
	}
	
	if (isOneTimeInteraction)
	{
		wasInteractedWith = true;
	}
	else
	{
		buttonState = State::Active;
	}
}
 
void AInteractableButton::PressButton()
{
	if (!isReturningFromPress)
	{
		if (AnimateButton(true, buttonAnimDuration))
		{
			elapsedTime = 0.0f;
			isReturningFromPress = true;
		}
		return;
	}

	if (AnimateButton(false, buttonAnimDuration))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		wasInteractedWith = isOneTimeInteraction;
		buttonState = wasInteractedWith ? State::Busy : State::Active;
		isReturningFromPress = false;
		
		if (IActivatable* Activatable = Cast<IActivatable>(TargetActor))
		{
			Activatable->Execute_SimpleActivate(TargetActor);
		}
	}
}

void AInteractableButton::PushButton()
{
	if (AnimateButton(isPushed, buttonAnimDuration))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		wasInteractedWith = isOneTimeInteraction;
		buttonState = wasInteractedWith ? State::Busy : State::Active;
		isReturningFromPress = false;
		
		if (IActivatable* Activatable = Cast<IActivatable>(TargetActor))
		{
			Activatable->Execute_Activate(TargetActor, isPushed);
		}
	}	
}

bool AInteractableButton::AnimateButton(bool pressed, float animDuration)
{
	float start = pressed ? 0.0f : -buttonPushDistance;
	float end = pressed ? -buttonPushDistance : 0.0f;
	
	elapsedTime += TickInterval;
	float Alpha = FMath::Clamp(elapsedTime / animDuration, 0.0f, 1.0f);
	
	float ZOffset = FMath::Lerp(start, end, Alpha);
	FVector location = ButtonPlateMesh->GetRelativeLocation();
	location.Z = ZOffset;
	
	ButtonPlateMesh->SetRelativeLocation(location);
	
	return elapsedTime >= animDuration;
}
