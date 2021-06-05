#include "UStaticMeshComponent.h"
#include <Panels/Console.h>

UStaticMeshComponent::UStaticMeshComponent(AActor* owner) : UComponent(owner)
{
	
}

void UStaticMeshComponent::Tick(float delta)
{
	M_LOG("Poop");
}

void UStaticMeshComponent::Draw()
{
	
}

