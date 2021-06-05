#include "UComponent.h"
#include "Actors/AActor.h"

UComponent::UComponent(AActor* owner)
{
	this->owner = owner;
}

void UComponent::Init()
{
}

void UComponent::Tick(float delta)
{
}
