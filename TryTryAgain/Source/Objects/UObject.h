#pragma once
#include <Reflect.h>

using namespace Reflect;

class UObject
{
public:
	float f;
	float f2;

	REFLECT(UObject, f, f2)
};

