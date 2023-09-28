#include "GameBoard/Tiles/TileBase.h"

#include "GameBoard/GameBoard.h"
#include "GameBoard/GameBoardUtils.h"
#include "GameBoard/HighlightSystem.h"
#include "GameBoard/Link.h"
#include "GameplaySystems/TWS_GameManager.h"
#include "Units/UnitBase.h"

UTileBase::UTileBase()
{
}

ULink* UTileBase::CreateLink(TObjectPtr<UTileBase> Source, TObjectPtr<UTileBase> Target)
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

void UTileBase::AddLink(ULink* link)
{
	// NullCheck source and target
	if (link->GetSource() == nullptr || link->GetTarget() == nullptr)
		return;
	Links.Add(link);
}

void UTileBase::AddLink(TObjectPtr<UTileBase> source, TObjectPtr<UTileBase> target)
{
	// NullCheck source and target
	if (source == nullptr || target == nullptr)
		return;
	Links.Add(CreateLink(source, target));
}

void UTileBase::RemoveLink(ULink* link)
{
	// NullCheck source and target
	if (link->GetSource() == nullptr || link->GetTarget() == nullptr)
		return;
	Links.Remove(link); 
}

void UTileBase::RemoveLink( TObjectPtr<UTileBase> target)
{
	// NullCheck target
	if (target == nullptr)
		return;

	if(Links.IsEmpty()) return;
	
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

ULink* UTileBase::GetLinkTo(TObjectPtr<UTileBase> target)
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


void UTileBase::DestroyTile()
{
	// Remove all links
	for (ULink* Link : GetLinks())
	{
		RemoveLink(Link);
	}
	
	// Destroy the tile
	DestroyComponent();
}

void UTileBase::SelectTile(bool bSelected)
{
	const TObjectPtr<UTileBase> SelectedTile = bSelected ? this : nullptr;
	ParentGameBoard->HighlightSystem->SetSelectedTile(SelectedTile);
}

void UTileBase::HoverTile(bool bHovered)
{
	ParentGameBoard->HighlightSystem->SetHoverTile(this);
}


