#pragma once

#include <unordered_map>

#include "Globals.h"
#include "../Characters/Hero.h"
#include "../UI/UserInterface.h"
#include "../Engine/Engine.h"

namespace ETG
{
    class DebugText;

    class GameManager
    {
    public:
        GameManager();
        ~GameManager();
        void Initialize();
        void ProcessEvents();
        [[nodiscard]] bool WindowHasFocus() const { return HasFocus; }

        //I might delete this later on 
        static bool IsRunning() { return Globals::Window->isOpen(); }
        void Update();
        void Draw();

    private:
        std::unique_ptr<Hero> Hero;
        Engine EngineUI{};

        bool HasFocus = true;
        std::unique_ptr<DebugText> DebugText;

    public:
        //Hold only scene objects. Used for displaying details panel
        std::unordered_map<std::string,GameObject*> SceneObjects;
        
        std::unique_ptr<UserInterface> UI;
        static sf::Event GameEvent;
    };
}
