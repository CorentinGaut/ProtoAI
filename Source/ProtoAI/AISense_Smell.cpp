// Fill out your copyright notice in the Description page of Project Settings.


#include "AISense_Smell.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "AISenseConfig_Smell.h"
#include "Perception/AIPerceptionComponent.h"

UAISense_Smell::UAISense_Smell()
{
	OnNewListenerDelegate.BindUObject(this, &UAISense_Smell::OnNewListenerImpl);
	OnListenerRemovedDelegate.BindUObject(this, &UAISense_Smell::OnListenerRemovedImpl);
}

float UAISense_Smell::Update()
{
	AIPerception::FListenerMap& ListenersMap = *GetListeners();
	//For each listener who has this sense we're going to perform a sweep to determine nearby aqua actors
	for (auto& Elem : ListenersMap)
	{
		//Get the listener
		FPerceptionListener Listener = Elem.Value;
		const AActor* ListenerBodyActor = Listener.GetBodyActor();

		for (int32 DigestedPropertyIndex = 0; DigestedPropertyIndex < DigestedProperties.Num(); DigestedPropertyIndex++)
		{
			//Create the sphere for this sense and perform the sweep to determine nearby actors
			FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(DigestedProperties[DigestedPropertyIndex].Radius);
			TArray<FHitResult> HitResults;
			GetWorld()->SweepMultiByChannel(HitResults, ListenerBodyActor->GetActorLocation(), ListenerBodyActor->GetActorLocation() + FVector::UpVector * CollisionSphere.GetSphereRadius(), FQuat(), ECollisionChannel::ECC_WorldDynamic, CollisionSphere);

			//Draw debug sphere if we have activated it via the config
			if (DigestedProperties[DigestedPropertyIndex].DisplayDebugSphere)
			{
				DrawDebugSphere(GetWorld(), ListenerBodyActor->GetActorLocation(), DigestedProperties[DigestedPropertyIndex].Radius, 8, FColor::Blue, false, 30.f, 1, 2.f);
			}


			//Check hit results for aqua actors
			for (int32 i = 0; i < HitResults.Num(); i++)
			{
				FHitResult hit = HitResults[i];
				//To simplify things, we're going to assume that "water resources" for this post are actors that have the following game tag
				if (hit.GetActor()->ActorHasTag(FName("SmellActor")))
				{
					if ((hit.GetActor()->GetActorLocation() - ListenerBodyActor->GetActorLocation()).Size() <= DigestedProperties[DigestedPropertyIndex].Radius)
					{
						Elem.Value.RegisterStimulus(hit.GetActor(), FAIStimulus(*this, 5.f, hit.GetActor()->GetActorLocation(), ListenerBodyActor->GetActorLocation()));
						GLog->Log("registered stimulus!");
					}

				}

			}
		}


	}
	//Time until next update; in this case we're forcing the update to happen in each frame
	return 0.f;
}

void UAISense_Smell::OnNewListenerImpl(const FPerceptionListener& NewListener)
{
	//Since we have at least one AI actor with this sense this function will fire when the game starts
	GLog->Log("hello new listener!");
	check(NewListener.Listener.IsValid());
	//Get the config
	UAISenseConfig* Config = NewListener.Listener->GetSenseConfig(GetSenseID());
	const UAISenseConfig_Smell* SenseConfig = Cast<const UAISenseConfig_Smell>(Config);
	check(SenseConfig);
	//Consume properties from the sense config
	FDigestedSmellProperties PropertyDigest(*SenseConfig);
	DigestedProperties.Add(PropertyDigest);
	RequestImmediateUpdate();
}

void UAISense_Smell::OnListenerRemovedImpl(const FPerceptionListener& UpdatedListener)
{
	//In our case, executes when we stop playing
	GLog->Log("on listener removed!");
}

UAISense_Smell::FDigestedSmellProperties::FDigestedSmellProperties()
{
	//Init.
	Radius = 15.0f;
	DisplayDebugSphere = false;
}

UAISense_Smell::FDigestedSmellProperties::FDigestedSmellProperties(const UAISenseConfig_Smell& SenseConfig)
{
	//Copy constructor
	Radius = SenseConfig.radius;
	DisplayDebugSphere = SenseConfig.DisplayDebugSphere;
}
