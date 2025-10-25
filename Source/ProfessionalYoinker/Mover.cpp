// Lycoris Bellua © All Rights Reserved

#include "Mover.h"
#include "Math/UnrealMathUtility.h"

UMover::UMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	TargetLocation = OriginalLocation + MoveOffset;
	Speed = MoveOffset.Length() / MoveTime;
}

void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation, NewLocation;

	CurrentLocation = GetOwner()->GetActorLocation();
	NewLocation = FMath::VInterpConstantTo(CurrentLocation, ShouldMove ? TargetLocation : OriginalLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(NewLocation);
	if (NewLocation == TargetLocation)
		ShouldMove = false;
}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
}
