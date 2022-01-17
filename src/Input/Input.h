#pragma once

#include <glm/glm.hpp>

class Input
{
public:
	static bool Keys[1024];

	static glm::ivec2 MouseRel;
	static bool MouseButtons[6];
	static bool MouseButtonsDown[6];
	static bool MouseButtonsUp[6];

	static float GetAxisUp();
	static float GetAxisRight();
	static float GetAxisForward();
};

