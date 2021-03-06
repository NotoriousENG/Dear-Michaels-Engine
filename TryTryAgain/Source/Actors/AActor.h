#pragma once

#include<Objects/UObject.h>
#include <string>
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

#include "Structs/FTransform.h"
#include <vector>
#include <Components/UComponent.h>
#include <memory>

class AActor : public UObject, public std::enable_shared_from_this<AActor>
{
public:

	typedef AActor Super;

    bool isExpandedInHierarchy;
	bool isUsing = false;
    bool isDead = false;
	
    std::shared_ptr<FTransform> transform;

    std::string name = "Actor";

	AActor();

	virtual void Init();

	virtual void Tick(float delta);

	template <typename T>
	T* AddComponent();

	UComponent* AddComponent(std::unique_ptr<UComponent> comp);

	template <typename T>
	T* GetComponent();

	UComponent* GetComponent(std::string name);

	template <typename T>
	void RemoveComponent();

	void RemoveComponent(std::string name);

	void ClearComponents();

	void ClearDescendents();

	glm::mat4 GetMVP();

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(name), CEREAL_NVP(transform), CEREAL_NVP(components));
	}

	std::vector<std::unique_ptr<UComponent>> components;
};

static void Destroy(AActor* actor)
{
	actor->ClearDescendents();
	actor->ClearComponents();
    actor->isDead = true;

	auto p = actor->transform->GetParent();
	if (p != nullptr)
	{
		// Remove child
		for (int i = 0; i < p->children.size(); i++)
		{
			if (p->children[i] == actor->transform)
			{
				p->children.erase(p->children.begin() + i);
				return;
			}
		}
	}
}

template<typename T>
inline T* AActor::AddComponent()
{
	static_assert(std::is_base_of<UComponent, T>::value, "Component must derive from UComponent");
	components.push_back(std::make_unique<T>(shared_from_this()));
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
		auto p = dynamic_cast<T*>(components[i].get());
		if (p != nullptr)
		{
			components.erase(components.begin() + i);
			return;
		}
	}
}

CEREAL_REGISTER_TYPE(AActor);