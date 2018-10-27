// Johan Hansson Carlberg 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
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

	// ...
	FindPhysicsHandleComponent();
	SetupInputComponent();


}

void UGrabber::FindPhysicsHandleComponent()
{
	//Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	//If physicshandle is found
	if (PhysicsHandle)
	{
		//TODO
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle component not found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	//Set inputcomponent
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	// Check for InputComponent
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent found on %s"), *GetOwner()->GetName());

		/// Bind the input actions 
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found on %s"), *GetOwner()->GetName());
	}
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//If the physics handle is attached
	/// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		//Move the object we're holding


	
	/// check if player is looking at a grabbable actor within distance
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

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

	return LineTraceHit;
}

// Ray-cast and grab what's in reach
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed!"));
	// Line trace and try to reach actors with physics body collision channel set
	FHitResult LineTraceHit = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = LineTraceHit.GetComponent();
	AActor* ActorToGrab = ComponentToGrab->GetOwner();
	if (LineTraceHit.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(LineTraceHit.GetComponent(), NAME_None, ActorToGrab->GetActorLocation(), ActorToGrab->GetActorRotation());
	}
	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
	//TODO Release physics handle
}



