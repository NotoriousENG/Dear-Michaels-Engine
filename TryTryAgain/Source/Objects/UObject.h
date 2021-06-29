#pragma once
#include <Reflect.h>
#include <Json.h>
#include <iostream>
#include <nameof.hpp>

using namespace Reflect;

template <typename T>
void Print(const T& t)
{		
	std::cout << nameof::nameof_short_type<decltype(t)>() << std::endl;
	std::cout << Json::pretty(t) << std::endl;
}

NOTE(UObject)
class UObject
{
public:

	float f = 0;
	float f2 = 0;

	virtual void Serialize() {
		Print<UObject>(*this);
	};

	REFLECT_NOTED(UObject, f, f2)
};

