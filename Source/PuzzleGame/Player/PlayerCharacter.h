#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PUZZLEGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	
	UPROPERTY(EditAnywhere, meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float moveSpeed = 1.0f;
	UPROPERTY(EditAnywhere, meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float cameraXSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, meta=(ClampMin="0.0", ClampMax="10.0", UIMin="0.0", UIMax="10.0"))
	float cameraYSensitivity = 1.0f;
	
	void InitializePlayerInput();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerInputs;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;

};
