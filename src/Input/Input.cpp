#include "Input.h"
#include <SDL2/SDL_keycode.h>

bool Input::Keys[1024];

glm::vec2 Input::MouseRel;
bool Input::MouseButtons[6];
bool Input::MouseButtonsDown[6];
bool Input::MouseButtonsUp[6];

float Input::GetAxisUp()
{
	return (int)Input::Keys[SDLK_e] - (int)Input::Keys[SDLK_q];
}

float Input::GetAxisRight()
{
	return (int)Input::Keys[SDLK_d] - (int)Input::Keys[SDLK_a];
}

float Input::GetAxisForward()
{
	return (int)Input::Keys[SDLK_w] - (int)Input::Keys[SDLK_s];
}
