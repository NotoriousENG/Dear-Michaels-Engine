#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <Serialization/CerealGLM.h>
#include <Panels/Console.h>
#include "Structs/FString.h"
#include <stack>
#include <memory>
#include <glm/gtx/matrix_decompose.hpp>

class AActor;

enum class AXIS
{
	x = 0,
	y = 1,
	z = 2,
	w = 3
};

class FTransform : public std::enable_shared_from_this<FTransform>
{
private:
	std::shared_ptr<FTransform> parent;
	
public:

	std::shared_ptr<AActor> actor;

	std::vector<std::shared_ptr<FTransform>> children;

	glm::vec3 localPosition = glm::vec3(0);
	glm::quat localRotation = glm::identity<glm::quat>();
	glm::vec3 localScale = glm::vec3(1);

	glm::mat4 GetModelMatrixWorld()
	{
		if (parent)
			return parent->GetModelMatrixWorld() * GetModelMatrixLocal();
		else
			return GetModelMatrixLocal();
	}

	glm::mat4 GetModelMatrixLocal()
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.0), localPosition);
		glm::mat4 rotate = glm::mat4_cast(localRotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0), localScale);

		return translate * rotate * scale;
	}

	glm::vec3 GetPosition()
	{
		glm::vec3 pos;

		glm::mat4 model = GetModelMatrixWorld();

		pos = glm::vec3(model[3][0], model[3][1], model[3][2]);

		return pos;
	}

	void SetPosition(glm::vec3 w_position)
	{
		if (parent)
		{
			glm::mat4 local = glm::translate(glm::inverse(parent->GetModelMatrixWorld()), w_position);

			localPosition = glm::vec3(local[3][0], local[3][1], local[3][2]);
		}
		else
		{
			localPosition = w_position;
		}
	}

	glm::quat GetRotation()
	{
		glm::vec3 scale;
		glm::quat rot;
		glm::vec3 pos;
		glm::vec3 skew;
		glm::vec4 persp;
		glm::decompose(GetModelMatrixWorld(), scale, rot, pos, skew, persp);

		return rot;
	}

	glm::vec3 GetScale()
	{
		glm::vec3 scale;
		glm::quat rot;
		glm::vec3 pos;
		glm::vec3 skew;
		glm::vec4 persp;
		glm::decompose(GetModelMatrixWorld(), scale, rot, pos, skew, persp);

		return scale;
	}

	void SetLocalFromWorld(glm::mat4 world)
	{
		glm::vec3 scale;
		glm::quat rot;
		glm::vec3 pos;
		glm::vec3 skew;
		glm::vec4 persp;

		if (parent)
		{
			auto local = glm::inverse(parent->GetModelMatrixWorld()) * world;
			glm::decompose(local, scale, rot, pos, skew, persp);
		}

		else
		{
			glm::decompose(world, scale, rot, pos, skew, persp);
		}

		localPosition = pos;
		localRotation = rot;
		localScale = scale;
	}

	std::shared_ptr<FTransform> GetParent()
	{
		return this->parent;
	}

	void SetParent(std::shared_ptr<FTransform> parent)
	{
		auto world = GetModelMatrixWorld();

		// remove this child from it's old parent
		if (this->parent != nullptr)
		{
			for (int i = 0; i < this->parent->children.size(); i++)
			{
				auto& c = this->parent->children[i];
				if (c.get() == this)
				{
					this->parent->children.erase(this->parent->children.begin() + i);
					break;
				}
			}
		}

		this->parent = parent;

		// maintain transformations in global space
		this->SetLocalFromWorld(world);
	}

	void AddChild(std::shared_ptr<FTransform> child)
	{
		if (this == child.get())
		{
			M_LOG("[warn]: parent and child can not be the same transform");
			return;
		}

		if (child == nullptr)
		{
			M_LOG("[warn]: child is null");
			return;
		}


		// DFS
		std::unordered_map<std::shared_ptr<FTransform>, bool> visited;
		std::stack<std::shared_ptr<FTransform>> s;
		s.push(child);
		while (!s.empty())
		{
			auto v = s.top();
			s.pop();
			if (v.get() == this)
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
		child->SetParent(shared_from_this());

		// the parent knows its new child
		children.push_back(child);

	}

	glm::vec3 getForward()
	{
		return glm::normalize(localRotation * glm::vec3(0, 0, 1));
	}

	std::shared_ptr<FTransform> GetRoot()
	{
		if (this->parent == nullptr)
			return shared_from_this();
		if (this->parent->parent == nullptr)
			return parent;

		return this->parent->GetRoot();
	}

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(localPosition), CEREAL_NVP(localRotation), CEREAL_NVP(localScale), CEREAL_NVP(actor), CEREAL_NVP(parent), CEREAL_NVP(children));
	}
};
