#pragma once

#include "DebugTexts.h"
#include "../Animation/AnimationManager.h"
#include "../Characters/Hero.h"
#include "../UI/UserInterface.h"
#include "../Engine/Engine.h"

namespace ETG
{
    using namespace Globals;

    class GameManager
    {
    private:
        std::unique_ptr<Hero> Hero;
        Engine EngineUI{};

        bool HasFocus = true;
        DebugText DebugText;

    public:
        //Hold only scene objects. Used for displaying details panel
        std::vector<GameObjectBase*> SceneObjects;
        
        std::unique_ptr<UserInterface> UI;
        static sf::Event GameEvent;


        void Initialize();
        void ProcessEvents();
        [[nodiscard]] bool WindowHasFocus() const { return HasFocus; }

        //I might delete this later on 
        static bool IsRunning() { return Window->isOpen(); }
        void Update();
        void Draw();
    };
}
