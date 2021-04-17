#pragma once

#include <string>
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

#include "Structs/FTransform.h"

class AActor
{
public:

	typedef AActor Super;

    bool isEditing;
    bool isDead = false;

	bool toDraw = true;
	
    FTransform transform;

    std::string name = "Actor";

	AActor(const char* name = "Actor", glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

	virtual void Draw();

	void DrawPicking();

	virtual void Tick(float delta);

	glm::mat4 model, view, projection, trans;

	unsigned int VBO, VAO, EBO;

protected:
	
	int numVertices;
};

static void Destroy(AActor* actor)
{
    actor->isDead = true;
}