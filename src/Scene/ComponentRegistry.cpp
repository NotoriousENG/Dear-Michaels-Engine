#include <ComponentRegistry.h>

#include <Components.h>

// INCLUDE COMPONENTS HERE //


void ComponentRegistry::Init()
{
	NameComponent::RegisterComponent();
	TransformComponent::RegisterComponent();
	StaticMeshComponent::RegisterComponent();
	SpriteComponent::RegisterComponent();

	// REGISTER COMPONENTS HERE //
	MovementComponent::RegisterComponent();


	ComponentRegistry::LogRegisteredComponents();
}

std::vector<std::string> ComponentRegistry::GetComponentNames()
{
	std::vector<std::string> componentNames;

	for (auto& kvp : registeredComponents)
	{
		componentNames.push_back(kvp.first);
	}

	return componentNames;
}

void ComponentRegistry::LogRegisteredComponents()
{
	printf("RegisteredComponents: [\n");
	for (auto& componentName : ComponentRegistry::GetComponentNames())
	{
		printf("\t%s,\n", componentName.c_str());
	}
	printf("]\n");
}
