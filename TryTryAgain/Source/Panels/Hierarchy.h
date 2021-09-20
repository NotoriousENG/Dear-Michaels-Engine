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

        void ShowActor(std::shared_ptr<AActor> actor);

        void ShowTransform(FTransform& transform);

        void AddActor();

        void Draw() override;
        void MenuItem() override;
    };
}
