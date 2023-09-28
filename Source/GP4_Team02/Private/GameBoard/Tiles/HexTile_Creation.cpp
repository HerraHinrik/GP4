// Fill out your copyright notice in the Description page of Project Settings.

#include "GameBoard/Tiles/HexTile_Creation.h"

#include "GameBoard/Link.h"

void UHexTile_Creation::AddLink(ULink* link)
{
	Super::AddLink(link);
	if(const TObjectPtr<UHexTile_Creation> connectedTile = Cast<UHexTile_Creation>(link->GetTarget()))
	{
		AddConnectedTile(connectedTile);
	}
}
