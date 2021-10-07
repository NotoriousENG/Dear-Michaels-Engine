#pragma once
#include "UComponent.h"

namespace rm {
    class Material;
    class Mesh;
}

enum class DRAW_TYPE
{
    MATERIAL = 0,
    PICKING = 1
};

class UStaticMeshComponent :
    public UComponent
{
public:

    typedef UComponent Super;

    std::shared_ptr<rm::Mesh> Mesh;

    std::shared_ptr<rm::Material> Material;

    UStaticMeshComponent(std::shared_ptr<AActor> owner);

    UStaticMeshComponent() {}

    virtual void Init() override;

    void Draw(DRAW_TYPE type = DRAW_TYPE::MATERIAL);

    virtual bool ShowInspector() override;

    bool bDraw = true;
};

