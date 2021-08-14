#include "AActor.h"

#include <glad/glad.h>

#include <glm/gtx/matrix_decompose.hpp>

#include "Elements/Game.h"

#include "ResourceManagement/ResourceManager.h"
#include <Panels/Console.h>
#include <Panels/Inspector.h>

AActor::AActor()
{
	Init();
}

void AActor::Init()
{
	for (auto& c : components)
	{
		c->Init();
	}
}

void AActor::UpdateMatrix()
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0), transform.position);
	glm::mat4 rotate = glm::mat4_cast(transform.rotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0), transform.scale);

	model = translate * rotate * scale;
}

void AActor::Tick(float delta)
{
	for (auto& c : components)
	{
		if (c->isActive)
			c->Tick(delta);
	}
	if (!isUsing)
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0), transform.position);
		glm::mat4 rotate = glm::mat4_cast(transform.rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), transform.scale);

		model = translate * rotate * scale;
	}
}

UComponent* AActor::AddComponent(std::unique_ptr<UComponent> comp)
{
	if (this->GetComponent(comp->name) == nullptr)
	{
		comp->owner = shared_from_this();
		components.push_back(std::move(comp));
		auto* p = components.back().get();
		p->Init();
		return p;
	}
	M_LOG("[error]: Component[%s] has already been added to Actor[%s]", comp->name.c_str(), this->name.c_str());
	return nullptr;
}

UComponent* AActor::GetComponent(std::string name)
{
	for (auto& c : components)
	{
		auto p = c.get();
		if (p->name == name)
		{
			return p;
		}
	}
	return nullptr;
}

void AActor::RemoveComponent(std::string name)
{
	for (int i = 0; i < components.size(); i++)
	{
		auto p = (components[i].get());
		if (p->name == name)
		{
			components.erase(components.begin() + i);
			return;
		}
	}
	M_LOG("[warn]: Cannot remove component[%s] as it does not belong to this Actor[%s]", 
		name.c_str(), this->name.c_str());
}

void AActor::ClearComponents()
{
	components.clear();
}

glm::mat4 AActor::GetMVP()
{
	return Game::MainCamera.projection * Game::MainCamera.view * this->model;
}