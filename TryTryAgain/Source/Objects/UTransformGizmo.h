#pragma once
#include "Structs/FTransform.h"

#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

class UTransformGizmo
{
public :

	FTransform SetTransform();

	FTransform GetTransform();

	void OnDrag()
	{
		
	}

	void Draw();

private:
	
	FTransform transform;
	
	glm::mat4 model, view, projection;

	unsigned int VBO, VAO, EBO;
};

