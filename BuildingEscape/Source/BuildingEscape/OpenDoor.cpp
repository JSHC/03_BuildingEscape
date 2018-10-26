// Johan Hansson Carlberg 2018

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Get the pawn of the first player controller and set it to the TriggerActor
	TriggerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//Poll the trigger volume
	//If the TriggerActor is in the trigger volume
	
	if (PressurePlate->IsOverlappingActor(TriggerActor) && bOpenDoorHasTriggered == false)
	{
		//Make sure the doors aren't already open (This is to not make the doors rotate on every frame)
		bOpenDoorHasTriggered = true;
		//Call the OpenDoor function
		OpenDoor();
	}
	
		
}


void UOpenDoor::OpenDoor()
{
	//Find Actor
	AActor* Owner = GetOwner();
	//Get current yaw
	float CurrYaw = Owner->GetActorRotation().Yaw;
	//Create new rotator with currentyaw-70
	FRotator NewRotation = FRotator(0.0f, CurrYaw - OpenAngle, 0.0f);
	//Set rotation
	Owner->SetActorRotation(NewRotation);
}
