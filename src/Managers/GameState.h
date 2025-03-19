#pragma once
#include <unordered_map>

#include "../Core/Scene/Scene.h"


namespace ETG
{
    class Hero;
    class Scene;

    class GameState
    {
    public:
        static GameState& GetInstance()
        {
            static GameState instance;
            return instance;
        }

        //TODO: The way getter and setters are completely incorrect. Getter should always return const reference whereas Set should allow object to be modified not set. The object's reference should never change.
        //TODO: But Set always only used once to set the reference which is always in GameManager. All these objects in this class has to be singleton but I don't know effectively how to handle making them singleton when declaration is nullptr.
        //TODO: I have to handle this after what I'm trying to do now. 

        void SetHero(Hero* hero) { MainHero = hero; }
        [[nodiscard]] Hero* GetHero() const { return MainHero; }

        [[nodiscard]] std::unordered_map<std::string, GameObjectBase*>& GetSceneObjs() { return *SceneObjs; }
        void SetSceneObjs(std::unordered_map<std::string, GameObjectBase*>& sceneObj) { SceneObjs = &sceneObj; }

        void SetEngineUISize(sf::Vector2f& size) { EngineUISize = std::ref(size); }
        [[nodiscard]] sf::Vector2f& GetEngineUISize() const { return EngineUISize.get(); }

        void SetSceneObj(Scene* sceneObj) { SceneObj = sceneObj; }
        [[nodiscard]] Scene* GetSceneObj() const { return SceneObj; }

        void SetRenderWindow(sf::RenderWindow* window) { Window = window; }
        [[nodiscard]] sf::RenderWindow* GetRenderWindow() const { return Window; }

    private:
        GameState() = default;
        Hero* MainHero = nullptr;

        //The object itself is at GameManager with the name SceneObjects
        //I am so sure this is extremely bad practice. Somehow I have to dictate singleton pattern that automatically initializes for the first time calling probably with static. Implement this later on. 
        std::unordered_map<std::string, GameObjectBase*>* SceneObjs = nullptr;
        Scene* SceneObj = nullptr;

        sf::Vector2f dummyEngineUISize{};
        std::reference_wrapper<sf::Vector2f> EngineUISize{dummyEngineUISize};
        sf::RenderWindow* Window;
    };
}
