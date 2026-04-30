#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PuzzleGame/Interactable/PickupItem.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USceneComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PUZZLEGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	
	static TObjectPtr<APlayerCharacter> Get(const TObjectPtr<UObject> WorldContextObject);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	bool CanPickupItem() const;
	void PickupItem(TObjectPtr<APickupItem> Item);
	TObjectPtr<APickupItem> GetPickupItem() const;
	void SocketItem(TObjectPtr<USceneComponent> SceneComponent);
	void DropItem();
	
protected:
	
	UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float moveSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float cameraXSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float cameraYSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, Category="Detection", meta=(ClampMin="1.0", ClampMax="1000.0", UIMin="1.0", UIMax="1000.0"))
	float lineTraceLength = 300.0f;
	
	void InitializePlayerInput();
	void InteractableDetection();
	void PlayerInteract(bool interact);
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerInputs;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> DropAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, Category="Interactable")
	TObjectPtr<AActor> CurrentInteractable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> HoldingPoint;
	
	UPROPERTY(VisibleAnywhere, Category="Item")
	TObjectPtr<APickupItem> CurrentHeldItem;
};
