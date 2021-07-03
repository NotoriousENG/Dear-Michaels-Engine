#pragma once
#include <Reflect.h>
#include <Json.h>
#include <iostream>
#include <nameof.hpp>
#include <unordered_map>

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

	using create_f = std::unique_ptr<UObject>();

	static void Register(const char* name, create_f* fp)
	{
		Registry()[name] = fp;
	}

	static std::unique_ptr<UObject> Instantiate(const char* name)
	{
		auto it = Registry().find(name);
		return it == Registry().end() ? nullptr : (it->second)();
	}

	template <typename D>
	struct Registrar
	{
		explicit Registrar(const char* name)
		{
			UObject::Register(name, &D::create);
		}
		// make non-copyable, etc.
	};

	float f;
	float f2;


	virtual void Serialize() {
		Print<UObject>(*this);
	};

	REFLECT_NOTED(UObject, f, f2);

	static std::unordered_map<std::string, create_f*>& Registry();
};

