// Lycoris Bellua © All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROFESSIONALYOINKER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGrabber();

protected: 
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Press();
	UFUNCTION(BlueprintCallable)
	void Release();

private: 
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400;
	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;
	UPROPERTY(EditAnywhere)
	float HoldDistance = 150;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	bool IsGrabbing = false;

	UPhysicsHandleComponent* GetPhysicsHandle() const;
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;
	void GrabComponent(UPrimitiveComponent* Component, AActor* Actor, FVector Location);
	void MovePhysicsHandleTarget();
	void ReleaseComponent();
};
