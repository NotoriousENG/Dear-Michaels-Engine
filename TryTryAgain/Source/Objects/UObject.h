#pragma once
#include <Reflect.h>
#include <iostream>

using namespace Reflect;

NOTE(UObject)
class UObject
{
public:
	float f;
	float f2;

	virtual void Serialize();

	REFLECT(UObject, f, f2)
};

