#include "ComponentFactory.h"

#include "Actors/AActor.h" // because we forward declare in UComponent

// Include All Components Here //
#include "Components/UPlayerControlComponent.h"
#include "Components/UStaticMeshComponent.h"

ComponentFactory ComponentFactory::instance;

ComponentFactory::ComponentFactory()
{
	Register("UStaticMeshComponent", []() {return std::make_unique<UStaticMeshComponent>(); });
	Register("UPlayerControlComponent", []() {return std::make_unique<UPlayerControlComponent>(); });
	// REGISTER COMPONENTS HERE //

	// Save to vector for easy access via imgui
	for (auto& pair : registry)
	{
		Keys.push_back(pair.first.c_str());
	}
}

void ComponentFactory::Register(std::string CompName, CreationFunction cFunc)
{
	if (registry[CompName] == nullptr)
	{
		registry[CompName] = cFunc;
	}
}

std::unique_ptr<UComponent> ComponentFactory::Create(std::string CompName)
{
	return registry[CompName]();
}