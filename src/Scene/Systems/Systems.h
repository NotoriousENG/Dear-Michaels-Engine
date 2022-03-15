#pragma once
#include <Scene/Components.h>
#include <Input/Input.h>

void PlayerMove(TransformComponent& transform, MovementComponent& move, glm::vec3 dir, float delta)
{
	transform.transform = glm::translate(transform.transform, dir * move.speed * delta);
}