#include "ComponentFactory.h"

#include "Actors/AActor.h" // because we forward declare in UComponent

#include "Components/UStaticModelComponent.h"

ComponentFactory::ComponentFactory()
{
	Register("UStaticModelComponent", []() {return std::make_unique<UStaticModelComponent>(); });
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