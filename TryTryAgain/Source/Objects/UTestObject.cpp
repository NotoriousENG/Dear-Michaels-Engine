#include "UTestObject.h"

void UTestObject::Serialize()
{
	using UClass = UTestObject::Class;

	printf("UTestObject\n");

	UTestObject::Supers::ForEach(*this, [&](auto superInfo, auto& superObj)
		{
			using Super = typename decltype(superInfo)::Type;
			Super::Class::ForEachField(superObj, [&](auto& field, auto& value) {
				std::cout << "  " << field.name << ": " << value << std::endl;
				});
		});

	UClass::ForEachField(*this, [&](auto& field, auto& value)
		{
			std::cout << "  " << field.name << ": " << value << std::endl;
		}
	);
	printf("\n");
}
