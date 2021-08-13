#pragma once
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp> // smart pointers
#include <cereal/types/vector.hpp> // std::vector

class UObject
{
public:
	virtual void Init() = 0;

	virtual void Tick(float delta) = 0;
};
