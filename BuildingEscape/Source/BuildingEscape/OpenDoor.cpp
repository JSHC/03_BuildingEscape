// Johan Hansson Carlberg 2018

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

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
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//Poll the trigger volume
	//If the total mass of actors in the volume is greater than some value
	if (GetTotalMassOfActorsOnPLate() >= 60.f)
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

float UOpenDoor::GetTotalMassOfActorsOnPLate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	//Find all overlapping actors'
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//iterate through them adding their mass to totalmass
	for (AActor* actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor on pressure plate: %s"),*actor->GetName())
		if (actor->FindComponentByClass<UPrimitiveComponent>())
		{
			TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return TotalMass;
}