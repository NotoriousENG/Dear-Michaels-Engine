#pragma once

#include <string>
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

#include "Structs/FTransform.h"

class AActor
{
public:

    bool isEditing;
    bool isDead = false;
	
    FTransform transform;

    std::string name = "Actor";

	AActor(const char* name = "Actor", glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

	virtual void Draw();

	virtual void Tick(float delta);
	
protected:

	glm::mat4 model, view, projection;

	unsigned int VBO, VAO, EBO;
};

static void Destroy(AActor* actor)
{
    actor->isDead = true;
}