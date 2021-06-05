#pragma once

#include <string>
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

#include "Structs/FTransform.h"
#include <vector>
#include <Components/UComponent.h>
#include <memory>

class AActor
{
public:

	typedef AActor Super;

    bool isEditing;
	bool isUsing = false;
    bool isDead = false;

	bool toDraw = true;
	
    FTransform transform;

    std::string name = "Actor";

	AActor(const char* name = "Actor", glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

	virtual void Draw();

	void DrawPicking(int id);

	virtual void Tick(float delta);

	glm::mat4 model;

	unsigned int VBO, VAO, EBO;

	template <typename T>
	UComponent* AddComponent()
	{
		static_assert(std::is_base_of<UComponent, T>::value, "Component must derive from UComponent");
		components.push_back(std::make_unique<T>(this));
		return components.back().get();
	}

	template <typename T>
	UComponent* GetComponent()
	{
		static_assert(std::is_base_of<UComponent, T>::value, "Component must derive from UComponent");
		for (auto& c : components)
		{
			if (typeid(c.get()) == typeid(T))
			{
				return c.get();
			}
		}
		return nullptr;
	}

	template <typename T>
	void RemoveComponent()
	{
		static_assert(std::is_base_of<UComponent, T>::value, "Component must derive from UComponent");
		for (int i = 0; i < components.size(); i++)
		{
			if (typeid(components[i].get()) == typeid(T))
			{
				components.erase(components.begin() + i);
				return;
			}
		}
	}

	void ClearComponents()
	{
		components.clear();
	}


protected:

	int numVertices;

	std::vector<std::unique_ptr<UComponent>> components;
};

static void Destroy(AActor* actor)
{
	actor->ClearComponents();
    actor->isDead = true;
}