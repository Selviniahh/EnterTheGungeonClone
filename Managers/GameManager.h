#pragma once

#include "../Animation/AnimationManager.h"
#include "../Characters/Hero.h"
#include "../UI/UserInterface.h"

namespace ETG
{
    using namespace Globals;

    class GameManager
    {
    private:
        //For now I don't have anything other than hero and UI  
        //Hero class
        Hero Hero{sf::Vector2f(0, 0)};
        bool HasFocus = true;

    public:
        UserInterface UI;

        void Initialize();
        void ProcessEvents();
        bool WindowHasFocus() const {return HasFocus;}

        //I might delete this later on 
        static bool IsRunning() { return Window->isOpen(); };
        void Update();
        void Draw();
    };
}
