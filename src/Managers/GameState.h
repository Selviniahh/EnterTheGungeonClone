#pragma once
#include "../Characters/Hero.h"

namespace ETG
{
    class GameState
    {
    public:
        static GameState& GetInstance()
        {
            static GameState instance;
            return instance;
        }

        void SetHero(Hero* hero) {MainHero = hero;}
        Hero* GetHero() const {return MainHero;}

    private:
        GameState() = default;
        Hero* MainHero = nullptr; 
    };
}
