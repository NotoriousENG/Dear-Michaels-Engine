#pragma once
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

class UObject
{
public:
	virtual void Init() = 0;

	virtual void Tick(float delta) = 0;
};
