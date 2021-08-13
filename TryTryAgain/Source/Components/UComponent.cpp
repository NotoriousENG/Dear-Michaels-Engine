#include "UComponent.h"
#include "Actors/AActor.h"

UComponent::UComponent(std::shared_ptr<AActor> owner)
{
	this->owner = owner;
}

void UComponent::Init()
{
}

void UComponent::Tick(float delta)
{
}
