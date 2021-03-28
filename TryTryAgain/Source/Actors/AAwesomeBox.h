#pragma once

#include "AActor.h"

class AAwesomeBox : public AActor
{
public:
	
	AAwesomeBox(const char* name = "AwesomeBox", glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1));

	virtual void Draw() override;

	void Tick(float delta) override;
};

