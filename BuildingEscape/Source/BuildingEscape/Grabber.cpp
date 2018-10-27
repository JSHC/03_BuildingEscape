// Johan Hansson Carlberg 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/Public/DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	/// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	//Log out to test
	/*UE_LOG(LogTemp, Warning, TEXT("Position: %s | Rotation: %s"), *PlayerViewPointLocation.ToString(),*PlayerViewPointRotation.ToString());*/
	//Draw a red trace in the world to visualise
	FVector PlayerSize = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorScale3D();
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation, 
		LineTraceEnd, 
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f);
	/// Setup query params
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	/// Ray-cast to player reach
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	if (LineTraceHit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *LineTraceHit.GetActor()->GetName());
	}
	
	/// check if player is looking at a grabbable actor within distance
}

