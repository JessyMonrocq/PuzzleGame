#include "PuzzleGame/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PuzzleGame/Interactable/Interactable.h"

#pragma region Base
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

	// Setup Holding Point (cuz apparently it doesn't work like camera...)
	if (!HoldingPoint)
	{
		HoldingPoint = NewObject<USceneComponent>(this, TEXT("Runtime Item Holding Point"));
		HoldingPoint->RegisterComponent();
		HoldingPoint->AttachToComponent(Camera ? Camera : GetRootComponent(),
		                                FAttachmentTransformRules::KeepRelativeTransform);
		HoldingPoint->SetRelativeLocation(FVector(75.0f, 33.0f, -33.0f));
		UE_LOG(LogTemp, Warning, TEXT("HoldingPoint was null on %s, created a runtime fallback."), *GetName());
	}

	CurrentHeldItem = nullptr;
	CurrentInteractable = nullptr;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractableDetection();
}
#pragma endregion

#pragma region PublicMethods
TObjectPtr<APlayerCharacter> APlayerCharacter::Get(const TObjectPtr<UObject> WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	return Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
}

#pragma region ItemInteraction
bool APlayerCharacter::CanPickupItem() const
{
	return !CurrentHeldItem;
}

void APlayerCharacter::PickupItem(TObjectPtr<APickupItem> Item)
{
	if (!Item || !HoldingPoint)
	{
		return;
	}

	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->SetItemPhysics(false);
	Item->AttachToComponent(HoldingPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CurrentHeldItem = Item;
}

TObjectPtr<APickupItem> APlayerCharacter::GetPickupItem() const
{
	if (CurrentHeldItem != nullptr)
	{
		return CurrentHeldItem;
	}
	return nullptr;
}

void APlayerCharacter::SocketItem(TObjectPtr<USceneComponent> SceneComponent)
{
	CurrentHeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentHeldItem->AttachToComponent(SceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CurrentHeldItem = nullptr;
}

void APlayerCharacter::DropItem()
{
	if (!CurrentHeldItem)
	{
		return;
	}

	// -- Method to prevent dropping item inside object
	UPrimitiveComponent* HeldItemCollision = Cast<UPrimitiveComponent>(CurrentHeldItem->GetRootComponent());
	if (!HeldItemCollision)
	{
		return;
	}

	const FBoxSphereBounds HeldItemBounds = HeldItemCollision->Bounds;
	const FCollisionShape DropCollisionShape = FCollisionShape::MakeBox(HeldItemBounds.BoxExtent * 0.95f);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(DropItem), false);
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(CurrentHeldItem);

	const bool bDropBlocked = GetWorld()->OverlapBlockingTestByChannel(
		HeldItemBounds.Origin,
		CurrentHeldItem->GetActorQuat(),
		HeldItemCollision->GetCollisionObjectType(),
		DropCollisionShape,
		QueryParams);

	if (bDropBlocked)
	{
		return;
	}
	// -- Method to prevent dropping item inside object

	CurrentHeldItem->SetItemPhysics(true);
	CurrentHeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentHeldItem = nullptr;
}
#pragma endregion
#pragma endregion

#pragma region ProtectedMethods
#pragma region PlayerInput
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

		if (DropAction)
		{
			EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &APlayerCharacter::DropItem);
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
#pragma endregion
#pragma region Interaction
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
#pragma endregion
#pragma endregion
