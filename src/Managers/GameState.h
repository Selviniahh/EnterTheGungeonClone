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

        void SetHero(Hero* hero) { MainHero = hero; }
        Hero* GetHero() const { return MainHero; }

        void SetSceneObj(std::vector<GameObject*>& sceneObj) { SceneObj = &sceneObj; }
        std::vector<GameObject*>& GetSceneObj() { return *SceneObj; }

    private:
        GameState() = default;
        Hero* MainHero = nullptr;
        std::vector<GameObject*>* SceneObj = nullptr;
    };
}
