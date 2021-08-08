#pragma once
#include "UComponent.h"
#include <memory>

namespace rm {
    class Shader;
}

using namespace std;

class UStaticMeshComponent : public UComponent
{

public:

    UStaticMeshComponent(AActor* owner);

	void Draw(rm::Shader* shader);

	bool bDraw = true;

private:
};
