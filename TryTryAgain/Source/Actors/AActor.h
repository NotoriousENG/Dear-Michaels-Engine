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
	
    FTransform transform;

    std::string name = "Actor";

	AActor(const char* name = "Actor", glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

	virtual void Save();

	virtual void Init();

	virtual void Tick(float delta);

	glm::mat4 model;

	template <typename T>
	T* AddComponent();

	template <typename T>
	T* GetComponent();

	template <typename T>
	void RemoveComponent();

	void ClearComponents();

	glm::mat4 GetMVP();


protected:

	FTransform InitTransform;

	std::vector<std::unique_ptr<UComponent>> components;
};

static void Destroy(AActor* actor)
{
	actor->ClearComponents();
    actor->isDead = true;
}

template<typename T>
inline T* AActor::AddComponent()
{
	static_assert(std::is_base_of<UComponent, T>::value, "Component must derive from UComponent");
	components.push_back(std::make_unique<T>(this));
	return static_cast<T*>(components.back().get());
}

template<typename T>
inline T* AActor::GetComponent()
{
	static_assert(std::is_base_of<UComponent, T>::value, "Component must derive from UComponent");
	for (auto& c : components)
	{
		auto p = dynamic_cast<T*>(c.get());
		if (p != nullptr)
		{
			return p;
		}
	}
	return nullptr;
}

template<typename T>
inline void AActor::RemoveComponent()
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
