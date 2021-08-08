#include "UStaticMeshComponent.h"
#include <Panels/Console.h>
#include "ResourceManagement/Mesh.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/ResourceManager.h"
#include "Actors/AActor.h"
#include <Elements/Game.h>

UStaticMeshComponent::UStaticMeshComponent(AActor* owner) : UComponent(owner)
{
	
}

void UStaticMeshComponent::Draw(rm::Shader* shader)
{
	if (bDraw)
	{
        
	}
}