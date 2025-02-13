#pragma once

#include "DebugTexts.h"
#include "GameState.h"
#include "../Animation/AnimationManager.h"
#include "../Characters/Hero.h"
#include "../UI/UserInterface.h"
#include "../Engine/UI/EngineUI.h"

namespace ETG
{
    using namespace Globals;

    class GameManager
    {
    private:
        Hero Hero{sf::Vector2f(0, 0)};
        EngineUI EngineUI{};

        bool HasFocus = true;
        DebugText DebugText;

    public:
        //Hold only scene objects. Used for displaying details panel
        std::vector<GameObject*> SceneObjects;

        UserInterface UI;

        void Initialize();
        void ProcessEvents();
        bool WindowHasFocus() const { return HasFocus; }

        //I might delete this later on 
        static bool IsRunning() { return Window->isOpen(); }
        void Update();
        void Draw();
    };
}
