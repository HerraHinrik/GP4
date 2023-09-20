#pragma once

#include "CoreMinimal.h"
#include "Link.h"

class AUnitBase;
/**
 * 
 */
class GP4_TEAM02_API Node
{
public:
	// Link functions
	static ULink* CreateLink(TObjectPtr<UTileBase> Source, TObjectPtr<UTileBase> Target);
	void AddLink(ULink* link);
	void AddLink(TObjectPtr<UTileBase> source, TObjectPtr<UTileBase> target);
	void RemoveLink(ULink* link);
	void RemoveLink(TObjectPtr<UTileBase> target);
	TArray<ULink*> GetLinks() { return Links; }
	ULink* GetLinkTo(TObjectPtr<UTileBase> target);

	// Virtual functions
	virtual FVector GetWorldLocation() { return FVector(0, 0, 0); }
	virtual FRotator GetWorldRotation() { return FRotator(0, 0, 0); }

	// DestroyNode
	virtual void DestroyNode() {}

	// Set/Get Unit
	void SetOccupyingUnit(AUnitBase* unit) { Unit = unit; }
	AUnitBase* GetOccupyingUnit() const { return Unit; }
	// Remove Unit
	void RemoveUnit() { Unit = nullptr; }

	// Virtual Destructor
	virtual ~Node() {}
	
protected:
	TArray<ULink*> Links = TArray<ULink*>();

	AUnitBase* Unit = nullptr;
	
};
