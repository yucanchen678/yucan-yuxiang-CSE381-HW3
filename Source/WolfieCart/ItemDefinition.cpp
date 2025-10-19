// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefinition.h"

/**
*	Creates and returns a copy of this Item Definition.
*	@return	a copy of the item.
*/
UItemDefinition* UItemDefinition::CreateItemCopy() const
{
	TObjectPtr<UItemDefinition> ItemCopy = NewObject<UItemDefinition>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemText = this->ItemText;
	ItemCopy->WorldMesh = this->WorldMesh;

	return ItemCopy;
}