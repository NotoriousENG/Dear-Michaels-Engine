#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <Serialization/CerealGLM.h>
#include <Panels/Console.h>
#include "Structs/FString.h"
#include <stack>

class AActor;

enum class AXIS
{
	x = 0,
	y = 1,
	z = 2,
	w = 3
};

class FTransform : std::enable_shared_from_this<FTransform>
{
public:
	glm::mat4 GetModelMatrix()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0), GetPosition());
		glm::mat4 rotate = glm::mat4_cast(GetRotation());
		glm::mat4 scale = glm::scale(glm::mat4(1.0), GetScale());

		return (parent != nullptr) ? scale * rotate * translate : translate * rotate * scale;
	}

	// Global Getters / Setters

	glm::vec3 GetPosition()
	{
		return (parent != nullptr) ? parent->position + localPosition : position;
	}

	void SetPosition(glm::vec3 position)
	{
		// if we have a parent, the local position is the difference from the parent's position
		this->position = position;

		this->localPosition = (parent != nullptr) ? position - parent->position : position;
	}

	void SetPosition(AXIS axis, float value)
	{
		int i = (int)axis;
		if (i >= 0 && i < 3)
		{
			this->position[i] = value;
			this->localPosition[i] = (parent != nullptr) ? parent->position[i] - this->position[i] : this->position[i];
		}
	}

	glm::quat GetRotation()
	{
		// rot = parent * local
		return (parent != nullptr) ? parent->rotation : rotation;
	}

	void SetRotation(glm::quat rotation)
	{
		if (parent == nullptr)
			this->rotation = glm::normalize(rotation);

		// localrot = parent * inv_local
		// this->localRotation = (parent != nullptr) ? glm::identity<glm::quat>() : glm::normalize(this->rotation);
	}


	glm::vec3 GetScale()
	{
		return (parent != nullptr) ? parent->scale * localScale : scale;
	}

	void SetScale(glm::vec3 scale)
	{
		this->scale = scale;

		this->localScale = (parent != nullptr) ? scale / parent->scale : scale;
	}

	// local getters / setters

	glm::vec3 GetLocalPosition()
	{
		return this->localScale;
	}

	void SetLocalPosition(glm::vec3 localPostion)
	{
		this->localPosition = localPostion;

		this->position = (parent != nullptr) ? parent->position + this->localPosition : this->localPosition;
	}

	glm::quat GetLocalRotation()
	{
		return this->localRotation;
	}

	void SetLocalRotation(glm::quat localRotation)
	{
		this->localRotation = localRotation;

		this->rotation = (parent != nullptr) ? glm::normalize(parent->GetRotation() * glm::inverse(this->localRotation)) : glm::normalize(this->localRotation);
	}

	glm::vec3 GetLocalScale()
	{
		return this->localScale;
	}

	void SetLocalScale(glm::vec3 localScale)
	{
		this->localScale = localScale;

		this->scale = (parent != nullptr) ? parent->scale + this->localScale : this->localScale;
	}
protected:
	glm::vec3 position = glm::vec3(0);
	glm::quat rotation = glm::identity<glm::quat>();
	glm::vec3 scale = glm::vec3(1);

	glm::vec3 localPosition = glm::vec3(0);
	glm::quat localRotation = glm::identity<glm::quat>();
	glm::vec3 localScale = glm::vec3(1);

	glm::vec3 skew;
	glm::vec4 perspective;

public:

	std::shared_ptr<AActor> actor;
	std::shared_ptr<FTransform> parent;
	std::vector<std::shared_ptr<FTransform>> children;

	static void AddChild(std::shared_ptr<FTransform> parent, std::shared_ptr<FTransform> child)
	{
		if (parent == child)
		{
			M_LOG("[warn]: parent and child can not be the same transform");
			return;
		}

		if (parent != nullptr)
		{
			// DFS
			std::unordered_map<std::shared_ptr<FTransform>, bool> visited;
			std::stack<std::shared_ptr<FTransform>> s;
			s.push(child);
			while (!s.empty())
			{
				auto v = s.top();
				s.pop();
				if (v == parent)
				{
					M_LOG("[warn] cycle");
					return;
				}
				if (!visited[v])
				{
					visited[v] = true;
					for (auto& c : v->children)
					{
						s.push(c);
					}
				}
			}
		}


		// remove this child from it's old parent
		if (child->parent != nullptr)
		{
			for (int i = 0; i < child->parent->children.size(); i++)
			{
				auto& c = child->parent->children[i];
				if (c == child)
				{
					child->parent->children.erase(child->parent->children.begin() + i);
					break;
				}
			}
		}

		// the child knows it's new parent
		child->parent = parent;

		if (parent != nullptr)
			// the parent knows its new child
			parent->children.push_back(child);

	}

	glm::vec3 getForward()
	{
		return glm::normalize(rotation * glm::vec3(0, 0, 1));
	}

	std::shared_ptr<FTransform> GetRoot()
	{
		if (this->parent->parent == nullptr)
			return parent;

		return this->parent->GetRoot();
	}

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(position), CEREAL_NVP(rotation), CEREAL_NVP(scale), CEREAL_NVP(skew), CEREAL_NVP(perspective));
	}
};
