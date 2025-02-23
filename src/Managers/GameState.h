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
        [[nodiscard]] Hero* GetHero() const { return MainHero; }

        [[nodiscard]] std::vector<GameObject*>& GetSceneObj() const { return *SceneObj; }
        void SetSceneObj(std::vector<GameObject*>& sceneObj) { SceneObj = &sceneObj; }

        void SetEngineUISize(sf::Vector2f& size) { EngineUISize = std::ref(size); }
        [[nodiscard]] sf::Vector2f& GetEngineUISize() const { return EngineUISize.get(); }

    private:
        GameState() = default;
        Hero* MainHero = nullptr;
        std::vector<GameObject*>* SceneObj = nullptr;

        sf::Vector2f dummyEngineUISize{};
        std::reference_wrapper<sf::Vector2f> EngineUISize{dummyEngineUISize};
    };
}
