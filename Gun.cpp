// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

}


// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Spawn Impact FX & damage actor
void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,Mesh,TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleFireSound,Mesh,TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;
	bool bHit = GunTrace(HitResult,ShotDirection);

	if (bHit) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect,HitResult.Location,ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ImpactSound,HitResult.Location);
		AActor* HitActor = HitResult.GetActor();
		AController* OwnerController = GetOwnerController();
		if (OwnerController && HitActor)
		{
			FPointDamageEvent DamageEvent(DamagePerShot,HitResult,ShotDirection,nullptr);
			HitActor->TakeDamage(DamagePerShot,DamageEvent,OwnerController,this);
		}
	}
}


// LineTrace shot
bool AGun::GunTrace(FHitResult& Hit,FVector& ShotDirection)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) return false;
	OwnerController->GetPlayerViewPoint(ViewLocation,ViewRotation);
	ShotDirection = -ViewRotation.Vector();
	FVector End = ViewLocation + ViewRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit,ViewLocation,End,ECollisionChannel::ECC_GameTraceChannel1,Params);
}


// Get Valid Controller
AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}