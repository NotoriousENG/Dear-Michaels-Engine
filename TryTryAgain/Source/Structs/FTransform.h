#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

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
};
