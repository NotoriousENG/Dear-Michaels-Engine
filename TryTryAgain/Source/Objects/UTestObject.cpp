#include "UTestObject.h"

namespace
{
    UObject::Registrar<UTestObject> Registrar("UTestObject");
}

//void UTestObject::Serialize(bool isClass)
//{
//	using UClass = UTestObject;
//
//	UClass::Supers::ForEach(*this, [&](auto superInfo, auto& superObj) {
//		using Super = typename decltype(superInfo)::Type;
//		Super::Class::ForEachField(superObj, [&](auto& field, auto& value) {
//			std::cout << "  " << field.name << ": " << value << std::endl;
//			});
//		});
//
//	UClass::Class::ForEachField(*this, [&](auto& field, auto& value)
//		{
//			std::cout << "  " << field.name << ": " << value << std::endl;
//		}
//	);