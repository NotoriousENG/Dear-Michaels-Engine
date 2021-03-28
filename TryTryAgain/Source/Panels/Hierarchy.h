#pragma once
#include <imgui.h>

#include "Actors/AActor.h"

class Game;

namespace Panels
{
    class Hierarchy
    {

    public:

        Hierarchy(Game* game);

        static void HelpIcon(const char* desc);

        void ShowActor(AActor* actor);

        void ShowTransform(FTransform &transform);

        void AddActor();

        void Draw(bool* p_open);

    private:

        Game* MyGame;
    	
    };
}
