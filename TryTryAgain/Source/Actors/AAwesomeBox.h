#pragma once

#include "AActor.h"

class AAwesomeBox : public AActor
{
public:
	AAwesomeBox();

	void Init() override;

	void Tick(float delta) override;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::make_nvp("AActor", cereal::virtual_base_class<AActor>(this)));
	}
};

CEREAL_REGISTER_TYPE(AAwesomeBox);

