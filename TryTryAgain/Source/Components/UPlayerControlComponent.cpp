#include "UPlayerControlComponent.h"
#include "Actors/AActor.h"
#include "Input/Input.h"

#include <SDL2/SDL_events.h>

#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

void UPlayerControlComponent::Init()
{
	this->name = "UPlayerControlComponent";
}

void UPlayerControlComponent::Tick(float delta)
{
	PlayerInput.x = (int)Input::Keys[SDLK_a] + -1 * (int)Input::Keys[SDLK_d];
	PlayerInput.y = (int)Input::Keys[SDLK_w] + -1 * (int)Input::Keys[SDLK_s];

	owner->transform->localPosition = owner->transform->localPosition + PlayerInput.y * owner->transform->getForward() * Speed * delta;
	owner->transform->localRotation = glm::rotate(owner->transform->localRotation, PlayerInput.x * TurnSpeed * delta, glm::vec3(0, 1, 0));
}

bool UPlayerControlComponent::ShowInspector()
{
	if (Super::ShowInspector())
	{
		ImGui::InputFloat2("Player Input", glm::value_ptr(PlayerInput), "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::InputFloat("Speed", &this->Speed);
		ImGui::InputFloat("TurnSpeed", &this->TurnSpeed);

		return true;
	}

	return false;

}
