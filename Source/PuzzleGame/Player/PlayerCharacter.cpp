#include "PuzzleGame/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "PuzzleGame/Interactable/Interactable.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Setup player camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractableDetection();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InitializePlayerInput();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
			                                   &APlayerCharacter::PlayerInteract, true);
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this,
			                                   &APlayerCharacter::PlayerInteract, false);
		}
	}
}

void APlayerCharacter::InitializePlayerInput()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !PlayerInputs)
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(PlayerInputs, 0);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementValue.Y * moveSpeed);
	AddMovementInput(RightDirection, MovementValue.X * moveSpeed);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	AddControllerYawInput(LookValue.X * cameraXSensitivity);
	AddControllerPitchInput(-LookValue.Y * cameraYSensitivity);
}

void APlayerCharacter::InteractableDetection()
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * lineTraceLength);

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		TObjectPtr<AActor> HitActor = Hit.GetActor();

		if (!HitActor.IsNull() && HitActor.GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			if (HitActor == CurrentInteractable)
			{
				return;
			}
			
			if (!CurrentInteractable.IsNull())
			{
				IInteractable::Execute_Highlight(CurrentInteractable, false);
			}
			
			IInteractable::Execute_Highlight(HitActor, true);
			CurrentInteractable = HitActor;
		}
		else
		{
			if (CurrentInteractable.IsNull())
			{
				return;
			}
			
			if (!CurrentInteractable.IsNull() && CurrentInteractable.GetClass()->ImplementsInterface(
				UInteractable::StaticClass()))
			{
				IInteractable::Execute_Highlight(CurrentInteractable, false);
				CurrentInteractable = nullptr;
			}
		}
	}
	else
	{
		if (!CurrentInteractable.IsNull() && CurrentInteractable.GetClass()->ImplementsInterface(
			UInteractable::StaticClass()))
		{
			IInteractable::Execute_Highlight(CurrentInteractable, false);
			CurrentInteractable = nullptr;
		}
	}
}

void APlayerCharacter::PlayerInteract(bool interact)
{
	if (!CurrentInteractable.IsNull())
	{
		if (IInteractable::Execute_IsHoldInteraction(CurrentInteractable))
		{
			IInteractable::Execute_HoldInteraction(CurrentInteractable, interact);
			// TODO : Block Player movement/camera if hold interaction
		}
		else if (interact)
		{
			IInteractable::Execute_Interact(CurrentInteractable);
		}
	}
}
