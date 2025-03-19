#pragma once
#include <unordered_map>
#include "Globals.h"
#include "../Engine/Engine.h"

namespace ETG
{
    class DebugText;
    class Scene;
    class Hero;
    class UserInterface;
    class PlatinumBullets;
    class BulletMan;

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
        std::unique_ptr<UserInterface> UI;
        std::unique_ptr<Scene> Scene;
        std::unique_ptr<BulletMan> BulletMan;

        std::unique_ptr<PlatinumBullets> PlatinumBullets; 
        
        Engine EngineUI{};

        bool HasFocus = true;
        std::unique_ptr<DebugText> DebugText;

    public:
        //Hold only scene objects. Used for displaying details panel
        std::unordered_map<std::string,GameObjectBase*> SceneObjects;
        
        static sf::Event GameEvent;
    };
}
