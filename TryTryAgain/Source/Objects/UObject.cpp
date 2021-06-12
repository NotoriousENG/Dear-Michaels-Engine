#include "UObject.h"

void UObject::Serialize()
{
	using UClass = UObject::Class;

	printf("UObject\n");

	UClass::ForEachField(*this, [&](auto& field, auto& value)
		{
			std::cout << "  " << field.name << ": " << value << std::endl;
		}
	);
	printf("\n");
}
