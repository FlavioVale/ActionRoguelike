// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//Creating the static mesh component and activating physics
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	BarrelMesh->SetSimulatePhysics(true);

	//Selecting the Collision Profile
	BarrelMesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = BarrelMesh;

	//Adding the explosion to the barrel
	Explosion = CreateDefaultSubobject<URadialForceComponent>("Explosion");
	Explosion->SetupAttachment(BarrelMesh);

	//Adding a small constant force to the component
	Explosion->SetAutoActivate(false);

	//Explosion parameters
	Explosion->Radius = 250.0f;
	Explosion->ImpulseStrength = 1500.0f;

	//Option to ignore the Mass of others objects(True = Impulse strength is the same for all, false = considers mass, needs more impulse to push some objects, something like 200000.0f)
	Explosion->bImpulseVelChange = true;

	//Adding the World collision channel to be affected
	Explosion->AddCollisionChannelToAffect(ECC_WorldDynamic);

	/*
	SphereRadius = 100.0f;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	SphereComp->InitSphereRadius(SphereRadius);
	SphereComp->SetCollisionProfileName("Trigger");
	RootComponent = SphereComp;

	ExplosiveBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	ExplosiveBarrel->SetupAttachment(RootComponent);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &K2_DestroyActor::AExplosiveBarrel::OnOverlapBegin);AddDynamic(this, &DestroyActorOnOverlap::OnOverlapBegin);
	*/
	
}

void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Hit component
	BarrelMesh->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);
}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Activating the explosion
	Explosion->FireImpulse();

	//Destroying the Barrel after the explosion
	BarrelMesh->DestroyComponent();
}


/*this is the old code
// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();


	//Destroy(ExplosiveBarrel);
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	
	 if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	 {
	 	EBTMemoryClear::Destroy();
	 }
	

}
*/