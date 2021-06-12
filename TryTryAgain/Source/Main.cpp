#include "Main.h"

#include <memory>
#include <SDL2/SDL.h>

#include "Application.h"
#include "Objects/UObject.h"
#include "Objects/UTestObject.h"

#include <iostream>

#undef main

int main(void)
{
	//// Initialize Application
	//auto app = std::make_unique<Application>("Michael's App");

	//// Start Application loop
	//app->Loop();

	UObject test;
	test.f = 1;
	test.f2 = 2;

	UTestObject test2;
	test2.f = 0;
	test2.f2 = 1;
	test2.f3 = 4;


	UObject::Class::ForEachField(test, [&](auto& field, auto& value) {
		std::cout << field.name << ": " << value << std::endl;
		});

	Reflect::count_supers(test2);

	UObject::Class::ForEachField(test2, [&](auto& field, auto& value) {
		std::cout << field.name << ": " << value << std::endl;
		});

	return 0;
}
