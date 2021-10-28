#pragma once
#include "Structs/FTransform.h"

#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

namespace rm
{
	class Shader;
}

class UTransformGizmo
{
public :

	UTransformGizmo();
	
	FTransform SetTransform();

	FTransform GetTransform();

	void OnDrag();

	void Draw();

private:
	
	FTransform transform;
	
	glm::mat4 model;

	unsigned int VBO, VAO, EBO;

	std::shared_ptr<rm::Shader> Shader;

	float vertices[66] =
	{
		0.0, 0.0, 0.0, 1.0 ,  1.0, 0.0, 0.0, 1.0,   0.0, 0.0, 1.0  ,
		5.0, 0.0, 0.0, 1.0 ,  1.0, 0.0, 0.0, 1.0 ,  0.0, 0.0, 1.0  ,
		0.0, 0.0, 0.0, 1.0 ,  0.0, 1.0, 0.0, 1.0 ,  0.0, 0.0, 1.0  ,
		0.0, 5.0, 0.0, 1.0 ,  0.0, 1.0, 0.0, 1.0 ,  0.0, 0.0, 1.0  ,
		0.0, 0.0, 0.0, 1.0 ,  0.0, 0.0, 1.0, 1.0 ,  0.0, 0.0, 1.0  ,
		0.0, 0.0, 5.0, 1.0 ,  0.0, 0.0, 1.0, 1.0 ,  0.0, 0.0, 1.0  ,
	};
};

