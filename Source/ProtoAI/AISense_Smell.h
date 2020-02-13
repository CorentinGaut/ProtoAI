// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense_Blueprint.h"
#include "AISense_Smell.generated.h"

/**
 * 
 */
class UAISenseConfig_Smell;

UCLASS(meta=(DisplayName="AI Smell Config"))
class PROTOAI_API UAISense_Smell : public UAISense_Blueprint
{
	GENERATED_BODY()

	public:
	struct FDigestedSmellProperties
	{
		float Radius;
		bool DisplayDebugSphere;

		FDigestedSmellProperties();
		FDigestedSmellProperties(const UAISenseConfig_Smell& SenseConfig);
	};

	TArray<FDigestedSmellProperties> DigestedProperties;
	UAISense_Smell();

	protected:
	virtual float Update() override;

	void OnNewListenerImpl(const FPerceptionListener& NewListener);

	void OnListenerRemovedImpl(const FPerceptionListener& UpdatedListener);
	
};
