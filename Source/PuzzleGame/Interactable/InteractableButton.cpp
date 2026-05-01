#include "PuzzleGame/Interactable/InteractableButton.h"
#include "Components/StaticMeshComponent.h"

AInteractableButton::AInteractableButton()
{
	PrimaryActorTick.bCanEverTick = false;
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

bool AInteractableButton::IsHoldInteraction_Implementation() const
{
	return false;
}

void AInteractableButton::Highlight_Implementation(bool detected)
{
	if (!detected || buttonState == State::Busy)
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
}
 
void AInteractableButton::PressButton()
{
	if (!isReturningFromPress)
	{
		if (AnimateButton(true, animDuration))
		{
			elapsedTime = 0.0f;
			isReturningFromPress = true;
			
			OnButtonPressed(true);
			if (IActivatable* Activatable = Cast<IActivatable>(TargetActor))
			{
				Activatable->Execute_SimpleInteract(TargetActor);
			}
		}
		return;
	}

	if (AnimateButton(false, animDuration))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		wasInteractedWith = isOneTimeInteraction;
		buttonState = wasInteractedWith ? State::Busy : State::Active;
		isReturningFromPress = false;
		
		OnButtonPressed(false);
	}
}

void AInteractableButton::PushButton()
{
	if (AnimateButton(isPushed, animDuration))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		wasInteractedWith = isOneTimeInteraction;
		buttonState = wasInteractedWith ? State::Busy : State::Active;
		isReturningFromPress = false;
		
		OnButtonPressed(isPushed);
		
		if (IActivatable* Activatable = Cast<IActivatable>(TargetActor))
		{
			Activatable->Execute_Interact(TargetActor, isPushed);
		}
	}	
}

bool AInteractableButton::AnimateButton(bool pressed, float duration)
{
	float start = pressed ? 0.0f : -buttonPushDistance;
	float end = pressed ? -buttonPushDistance : 0.0f;
	
	elapsedTime += TickInterval;
	float Alpha = FMath::Clamp(elapsedTime / duration, 0.0f, 1.0f);
	
	float ZOffset = FMath::Lerp(start, end, Alpha);
	FVector location = ButtonPlateMesh->GetRelativeLocation();
	location.Z = ZOffset;
	
	ButtonPlateMesh->SetRelativeLocation(location);
	
	return elapsedTime >= duration;
}
