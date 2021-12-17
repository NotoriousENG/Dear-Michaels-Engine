#pragma once
#include "UComponent.h"
#include <glm/vec2.hpp>
class UPlayerControlComponent : public UComponent
{
public:
	typedef UComponent Super;
	float Speed = 1;
	float TurnSpeed = 1;
	// Inherited via UComponent
	virtual void Init() override;
	virtual void Tick(float delta) override;
	virtual bool ShowInspector() override;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::make_nvp("UComponent", cereal::virtual_base_class<UComponent>(this)), CEREAL_NVP(Speed), CEREAL_NVP(TurnSpeed));
	}

protected:

	glm::vec2 PlayerInput;
	
};

CEREAL_REGISTER_TYPE(UPlayerControlComponent);