#include "UTestObject.h"

void UTestObject::Serialize()
{
	using UClass = UTestObject::Class;

	printf("UTestObject\n");

	UClass::ForEachField(*this, [&](auto& field, auto& value)
		{
			std::cout << "  " << field.name << ": " << value << std::endl;
		}
	);
	printf("\n");
}
