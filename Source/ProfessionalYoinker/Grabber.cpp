// Lycoris Bellua © All Rights Reserved

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetPhysicsHandle();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsGrabbing)
		MovePhysicsHandleTarget();
}

void UGrabber::Press()
{
	//UE_LOG(LogTemp, Display, TEXT("Grabber button: Pressed"));
	if (PhysicsHandle == nullptr)
		return;

	FHitResult HitResult;
	if (GetGrabbableInReach(HitResult))
	{
		GrabComponent(HitResult.GetComponent(), HitResult.GetActor(), HitResult.ImpactPoint);
		IsGrabbing = true;
	}
}

void UGrabber::Release()
{
	//UE_LOG(LogTemp, Display, TEXT("Grabber button: Released"));
	if (PhysicsHandle == nullptr)
		return;

	ReleaseComponent();
	IsGrabbing = false;
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("The player character is missing a Physics Handle!"));
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	//DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	bool HasHit = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
	/*
	if (HasHit)
	{
		DrawDebugSphere(GetWorld(), OutHitResult.Location, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		AActor* HitActor = OutHitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("Hit actor: %s"), *HitActor->GetActorNameOrLabel());
	}
	*/
	return HasHit;
}

void UGrabber::GrabComponent(UPrimitiveComponent* Component, AActor* Actor, FVector Location)
{
	if (Component->IsSimulatingPhysics() == false)
		Component->SetSimulatePhysics(true);
	Component->WakeAllRigidBodies();
	Component->SetAngularDamping(50);
	Actor->Tags.Add("Grabbed");
	Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PhysicsHandle->GrabComponentAtLocationWithRotation(Component, NAME_None, Location, GetComponentRotation());
}

void UGrabber::MovePhysicsHandleTarget()
{
	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabber::ReleaseComponent()
{
	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (GrabbedComponent == nullptr)
		return;

	GrabbedComponent->WakeAllRigidBodies();
	GrabbedComponent->SetAngularDamping(0);
	GrabbedComponent->GetOwner()->Tags.Remove("Grabbed");
	PhysicsHandle->ReleaseComponent();
}
