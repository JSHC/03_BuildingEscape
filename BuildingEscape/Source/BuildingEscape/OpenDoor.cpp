// Johan Hansson Carlberg 2018

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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
		UE_LOG(LogTemp, Warning, TEXT("Open Door Trigger Called"));
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
