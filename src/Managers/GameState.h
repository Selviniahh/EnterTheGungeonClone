#pragma once
#include <unordered_map>
#include "../Core/Scene/Scene.h"
#include "../Items/Active/ActiveItemBase.h"
#include "../Items/Passive/PassiveItemBase.h"

namespace ETG
{
    class Hero;
    class Scene;
    class GameObjectBase;
    class PassiveItemBase;


    class GameState
    {
    public:
        static GameState& GetInstance()
        {
            static GameState instance;
            return instance;
        }

        [[nodiscard]] Hero* GetHero() const { return MainHero; }
        [[nodiscard]] std::unordered_map<std::string, GameObjectBase*>& GetSceneObjs() const { return *SceneObjs; }
        [[nodiscard]] sf::Vector2f& GetEngineUISize() { return EngineUISize; }
        [[nodiscard]] Scene* GetSceneObj() const { return SceneObj; }
        [[nodiscard]] sf::RenderWindow* GetRenderWindow() const { return Window; }
        [[nodiscard]] std::vector<PassiveItemBase*>& GetEquippedPassiveItems() { return EquippedPassiveItems; }
        [[nodiscard]] std::vector<ActiveItemBase*>& GetEquippedActiveItems() { return EquippedActiveItems; }

        void SetHero(Hero* hero) { MainHero = hero; }
        void SetSceneObjs(std::unordered_map<std::string, GameObjectBase*>& sceneObj) { SceneObjs = &sceneObj; }
        void SetEngineUISize(sf::Vector2f& size) { EngineUISize = std::ref(size); }
        void SetSceneObj(Scene* sceneObj) { SceneObj = sceneObj; }
        void SetRenderWindow(sf::RenderWindow* window) { Window = window; }

        //TODO: Who should own equipped items class? Hero cannot because hero also don't know which item it's interacted. UI or GameManager might but for now I will let this class to own equipped items.
        // void SetEquippedPassiveItems(const std::vector<PassiveItemBase*>& eqPassiveItem) { EquippedPassiveItems = eqPassiveItem; }

    private:
        GameState() = default;
        Hero* MainHero = nullptr;

        //Game objects (non-owning pointers)
        std::unordered_map<std::string, GameObjectBase*>* SceneObjs = nullptr;
        Scene* SceneObj = nullptr;
        sf::RenderWindow* Window;
        
        //Owned objects
        sf::Vector2f EngineUISize{};
        std::vector<PassiveItemBase*> EquippedPassiveItems{};
        std::vector<ActiveItemBase*> EquippedActiveItems{};
    };
}
