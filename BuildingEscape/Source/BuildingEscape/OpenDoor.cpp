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
	//Set the owning door
	Owner = GetOwner();

	StartAngle = Owner->GetActorRotation().Yaw;
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
	
	if (PressurePlate->IsOverlappingActor(TriggerActor))
	{
		//Call the OpenDoor function
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("LastDoorOpenTime: %f"), LastDoorOpenTime);
	}
	if (GetWorld()->GetTimeSeconds() - DoorCloseDelay >= LastDoorOpenTime)
	{
		//Close the door
		CloseDoor();
	}
	GetWorld()->GetTimeSeconds();
		
}


void UOpenDoor::OpenDoor()
{
	//Owner->SetActorRotation(FRotator(0.0f, CurrYaw - OpenAngle, 0.0f));
	Owner->SetActorRotation(FRotator(0.0f, StartAngle-OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	//Owner->SetActorRotation(FRotator(0.0f, CurrYaw + OpenAngle, 0.0f));
	Owner->SetActorRotation(FRotator(0.0f, StartAngle, 0.0f));
}