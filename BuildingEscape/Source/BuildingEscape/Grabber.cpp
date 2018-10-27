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
	//Check if we're holding a component
	if (PhysicsHandle->GrabbedComponent)
	{
		//Update the location of the grabbed component to the end of the reach
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Setup query params
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	/// Ray-cast to player reach
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	if (LineTraceHit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *LineTraceHit.GetActor()->GetName());
	}

	return LineTraceHit;
}

FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	return LineTraceEnd;
}

void UGrabber::Grab()
{
	// Line trace and try to reach actors with physics body collision channel set
	UE_LOG(LogTemp,Warning, TEXT("Grab key pressed"))
	FHitResult LineTraceHit = GetFirstPhysicsBodyInReach();
	//Check if actor isn't null to avoid crashes
	if (LineTraceHit.GetActor() != nullptr)
	{
		//Grab the component at it's location without rotating it
		PhysicsHandle->GrabComponentAtLocationWithRotation(LineTraceHit.GetComponent(), 
			NAME_None, 
			LineTraceHit.GetComponent()->GetOwner()->GetActorLocation(), 
			LineTraceHit.GetComponent()->GetOwner()->GetActorRotation());
	}
	
}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}



