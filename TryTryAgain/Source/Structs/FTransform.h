#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <Serialization/CerealGLM.h>

struct FTransform
{
	glm::vec3 position = glm::vec3(0);
	glm::quat rotation = glm::identity<glm::quat>();
	glm::vec3 scale = glm::vec3(1);

	glm::vec3 skew;
	glm::vec4 perspective;

	glm::vec3 getForward()
	{
		return glm::normalize(rotation * glm::vec3(0, 0, 1));
	}

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(position), CEREAL_NVP(rotation), CEREAL_NVP(scale), CEREAL_NVP(skew), CEREAL_NVP(perspective));
	}
};
