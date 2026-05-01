#include "PuzzleGame/Activatable/SteamWall.h"

#include "Components/BoxComponent.h"

ASteamWall::ASteamWall()
{
	PrimaryActorTick.bCanEverTick = false;

	SteamWallCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("SteamWallCollider"));
	SetRootComponent(SteamWallCollider);
	SteamWallCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASteamWall::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AttachSteamParticlesToRoot();
}

void ASteamWall::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttachSteamParticlesToRoot();
}

void ASteamWall::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsPoweredOnStart)
	{
		currentPower = requiredPower;
		isPowered = true;
		if (InteractOnPowered)
		{
			Interact_Implementation(true);
		}
	}
	else
	{
		currentPower = 0;
		isPowered = false;
	}
}

void ASteamWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASteamWall::SetPower_Implementation(bool powerUp)
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
			Interact_Implementation(true);
		}
	}
	else if (currentPower < requiredPower && isPowered)
	{
		Interact_Implementation(false);
		isPowered = false;
	}
}

void ASteamWall::Interact_Implementation(bool state)
{
	SteamWallCollider->SetCollisionEnabled(state ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	
	for (TObjectPtr<UNiagaraComponent> Particle : SteamParticles)
	{
		if (state)
		{
			Particle->Deactivate();
		} else
		{
			Particle->Activate(true);
		}
	}
}

void ASteamWall::AttachSteamParticlesToRoot()
{
	USceneComponent* RootSceneComponent = GetRootComponent();
	if (!RootSceneComponent)
	{
		return;
	}

	for (UNiagaraComponent* SteamParticle : SteamParticles)
	{
		if (!SteamParticle || SteamParticle->GetAttachParent() == RootSceneComponent)
		{
			continue;
		}

		if (SteamParticle->IsRegistered())
		{
			SteamParticle->AttachToComponent(RootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else
		{
			SteamParticle->SetupAttachment(RootSceneComponent);
		}
	}
}



