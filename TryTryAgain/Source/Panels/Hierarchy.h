#pragma once
#include <imgui.h>

#include "IPanel.h"
#include "Actors/AActor.h"

class Game;

namespace Panels
{
    class Hierarchy : public IPanel
    {

    public:

        static void HelpIcon(const char* desc);

        void ShowActor(AActor* actor);

        void ShowTransform(FTransform& transform);

        void AddActor();

        void Draw() override;
        void MenuItem() override;
    };
}
