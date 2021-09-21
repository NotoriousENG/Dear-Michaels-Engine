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

	glm::mat4 GetModelMatrixWorld()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0), GetPosition());
		glm::mat4 rotate = glm::mat4_cast(GetRotation());
		glm::mat4 scale = glm::scale(glm::mat4(1.0), GetScale());

		return translate * rotate * scale;
	}

	glm::mat4 GetModelMatrixLocal()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0), localPosition);
		glm::mat4 rotate = glm::mat4_cast(localRotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), localScale);

		return translate * rotate * scale;
	}

	// Global Getters / Setters

	glm::vec3 GetPosition()
	{
		position = (parent != nullptr) ? parent->position + parent->rotation * (parent->scale * localPosition) : localPosition;
		return position;
	}

	void SetPosition(glm::vec3 position)
	{
		this->localPosition = (parent != nullptr) ? glm::inverse(parent->rotation) * (position - parent->position) / parent->scale : position;

		this->position = position;
	}

	glm::quat GetRotation()
	{
		rotation = (parent != nullptr) ? parent->rotation * localRotation : localRotation;
		return rotation;
	}

	void SetRotation(glm::quat rotation)
	{
		// localrot = rot * in_local
		this->localRotation = (parent != nullptr) ? rotation * glm::inverse(parent->rotation) : glm::normalize(rotation);

		this->rotation = rotation;
	}


	glm::vec3 GetScale()
	{
		scale = (parent != nullptr) ? parent->scale * localScale : localScale;
		return scale;
	}

	void SetScale(glm::vec3 scale)
	{
		this->localScale = (parent != nullptr) ? scale / parent->scale : scale;

		this->scale = scale;
	}

	// local getters / setters

	glm::vec3 GetLocalPosition()
	{
		return this->localPosition;
	}

	void SetLocalPosition(glm::vec3 localPostion)
	{
		this->localPosition = localPostion;

		this->position = GetPosition();
	}

	glm::quat GetLocalRotation()
	{
		return this->localRotation;
	}

	void SetLocalRotation(glm::quat localRotation)
	{
		this->localRotation = localRotation;

		this->rotation = GetRotation();
	}

	glm::vec3 GetLocalScale()
	{
		return this->localScale;
	}

	void SetLocalScale(glm::vec3 localScale)
	{
		this->localScale = localScale;

		this->scale = GetScale();
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
		ar(CEREAL_NVP(localPosition), CEREAL_NVP(localRotation), CEREAL_NVP(localScale), CEREAL_NVP(skew), CEREAL_NVP(perspective));
	}
};
