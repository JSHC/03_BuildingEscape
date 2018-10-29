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

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("No TriggerVolume found on %s (%s class)"), *Owner->GetName(), *this->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//Poll the trigger volume
	//If the total mass of actors in the volume is greater than some value
	if (GetTotalMassOfActorsOnPLate() >= TriggerMass)
	{
		///Broadcast the OnOpenRequest event
		OnOpen.Broadcast();
	}
	else
	{
		//Close the door
		OnClose.Broadcast();
	}
		
}

float UOpenDoor::GetTotalMassOfActorsOnPLate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate)
	{
		return TotalMass;
	}
	else
	{
		//Find all overlapping actors'
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);
		//iterate through them adding their mass to totalmass
		for (AActor* actor : OverlappingActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor on pressure plate: %s"), *actor->GetName())
				TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return TotalMass;
}