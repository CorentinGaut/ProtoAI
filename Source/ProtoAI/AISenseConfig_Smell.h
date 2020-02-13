// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISenseConfig_Blueprint.h"
#include "AISense_Smell.h"
#include "AISenseConfig_Smell.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "AI Smell Config"))
class PROTOAI_API UAISenseConfig_Smell : public UAISenseConfig_Blueprint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", NoClear, Config)
		TSubclassOf<UAISense_Smell> implement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", NoClear, Config)
		float radius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", NoClear, Config)
		bool DisplayDebugSphere;

	UAISenseConfig_Smell(const FObjectInitializer& ObjectInitializer);

	virtual TSubclassOf<UAISense> GetSenseImplementation() const override;
};
