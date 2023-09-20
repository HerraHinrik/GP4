
#include "GameBoard/Node.h"


ULink* Node::CreateLink(TObjectPtr<UTileBase> Source, TObjectPtr<UTileBase> Target)
{
	ULink* Link = NewObject<ULink>();

	// Check if the object was created successfully
	if (Link)
	{
		// Set source and target pointers
		Link->SetSource(Source);
		Link->SetTarget(Target);
	}
	else
	{
		// Print an error message
		UE_LOG(LogTemp, Error, TEXT("Failed to create link!"));
	}
	return Link;
}

void Node::AddLink(ULink* link)
{
	// NullCheck source and target
	if (link->GetSource() == nullptr || link->GetTarget() == nullptr)
		return;
	Links.Add(link);
}

void Node::AddLink(TObjectPtr<UTileBase> source, TObjectPtr<UTileBase> target)
{
	// NullCheck source and target
	if (source == nullptr || target == nullptr)
		return;
	Links.Add(CreateLink(source, target));
}

void Node::RemoveLink(ULink* link)
{
	// NullCheck source and target
	if (link->GetSource() == nullptr || link->GetTarget() == nullptr)
		return;
	Links.Remove(link); 
}

void Node::RemoveLink( TObjectPtr<UTileBase> target)
{
	// NullCheck target
	if (target == nullptr)
		return;

	// Loop through the links and remove the links that have the target as their target
	for (int32 i = 0; i < Links.Num(); i++)
	{
		ULink* Link = Links[i];
		if (Link->GetTarget() == target)
		{
			Links.Remove(Link);
			i--;
		}
	}

}

ULink* Node::GetLinkTo(TObjectPtr<UTileBase> target)
{
	// NullCheck target
	if (target == nullptr)
		return nullptr;
	for (ULink* Link : Links)
	{
		if(Link->GetTarget() == target)
			return Link;
	}
	return nullptr;
}
