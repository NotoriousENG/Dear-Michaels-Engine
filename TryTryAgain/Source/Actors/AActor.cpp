#include "AActor.h"

AActor::AActor(const char* name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	this->name = name;
	this->transform = FTransform
	{
		pos,
		rot,
		scale
	};
}

void AActor::Draw()
{
	
}

void AActor::Tick(float delta)
{

}
