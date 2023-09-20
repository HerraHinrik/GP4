// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Link.generated.h"

class UTileBase;

UCLASS()
class GP4_TEAM02_API ULink : public UObject
{
	GENERATED_BODY()
	
public:

	// Constructor declaration
	ULink();
	ULink(const FObjectInitializer& ObjectInitializer);

	// Getters
	TObjectPtr<UTileBase> GetSource() const { return Source;}
	TObjectPtr<UTileBase> GetTarget() const { return Target;}

	// Setters
	void SetSource(TObjectPtr<UTileBase> source) { Source = source; }
	void SetTarget(TObjectPtr<UTileBase> target) { Target = target; }

	
private:
	TObjectPtr<UTileBase> Source = nullptr;
	TObjectPtr<UTileBase> Target = nullptr;
};
