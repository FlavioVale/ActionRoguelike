// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/SphereComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	SphereRadius = 100.0f;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	SphereComp->InitSphereRadius(SphereRadius);
	SphereComp->SetCollisionProfileName("Trigger");
	RootComponent = SphereComp;

	ExplosiveBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	ExplosiveBarrel->SetupAttachment(RootComponent);



	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &K2_DestroyActor::AExplosiveBarrel::OnOverlapBegin);AddDynamic(this, &DestroyActorOnOverlap::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	// create tarray for hit results
	TArray<FHitResult> OutHits;

	// get actor locations
	FVector MyLocation = GetActorLocation();

	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(500.0f);

	// draw collision sphere
	DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Cyan, true);

	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, MyLocation, MyLocation, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((Hit.GetActor())->GetRootComponent());

			if (MeshComp)
			{
				// alternatively you can use  ERadialImpulseFalloff::RIF_Linear for the impulse to get linearly weaker as it gets further from origin.
				// set the float radius to 500 and the float strength to 2000.
				MeshComp->AddRadialImpulse(GetActorLocation(), 500.f, 2000.f, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}

	//Destroy(ExplosiveBarrel);
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	 *if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	 *{
	 *	EBTMemoryClear::Destroy();
	 *}
	*/

}