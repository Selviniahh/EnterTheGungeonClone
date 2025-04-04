#pragma once
#include <memory>
#include "../Managers/GameState.h"

class Engine;

//Factory global functions: 
namespace ETG
{
    //Forward declarations
    void RegisterGameObject(const std::string& name, GameObjectBase* obj);
    void UnregisterGameObject(const std::string& name);

    //NOTE: So important. Implemented Factory Method.
    //It's required to first construct the object, afterwards call some functions automatically for all game objects. Calling stuffs in Constructor of the base class will not be applicable for RTTI
    //Because RTTI will only retain base class' metadata. Employing factory method will let me set object name easily based on callee class type name.
    //Force to set Owner to be Scene
    //THe bottom overloaded template function needs to be invoked if GameObject is given. This function should be invoked if first argument only is not GameObject
    //SFIANE needs to be implemented for this purpose. 
    template <typename T, typename... Args>
    std::unique_ptr<T> CreateGameObjectDefault(Args&&... args)
    {
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        obj->Owner = GameState::GetInstance().GetSceneObj();

        const std::string objName = obj->SetObjectNameToSelfClassName();
        RegisterGameObject(objName, obj.get());
        return obj;
    }

    template <typename T, typename... Args>
    std::unique_ptr<T> CreateGameObjectAttached(GameObjectBase* OwnerObj, Args&&... args)
    {
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        obj->Owner = OwnerObj;

        // After full construction, set the object name based on its true dynamic type name and Set obj to unordered_map and vector
        const std::string objName = obj->SetObjectNameToSelfClassName();
        RegisterGameObject(objName, obj.get());
        return obj;
    }

    //Insert element to unordered_map and vector

    inline void RegisterGameObject(const std::string& name, GameObjectBase* obj)
    {
        auto& sceneObjs = GameState::GetInstance().GetSceneObjs();
        sceneObjs[name] = obj;

        auto& orderedObjList = GameState::GetInstance().GetOrderedSceneObjs();
        orderedObjList.push_back(obj);
    }

    //For now this function is only for updating hierarchy tab for removed game objects. 
    inline void UnregisterGameObject(const std::string& name)
    {
        auto& sceneObjs = GameState::GetInstance().GetSceneObjs();
        auto* obj = sceneObjs[name];

        //Remove from scene objects unordered_map firstly 
        sceneObjs.erase(name);

        //Remove from ordered list
        auto& orderedObjList = GameState::GetInstance().GetOrderedSceneObjs();
        const auto it = std::ranges::remove(orderedObjList, obj).begin(); //Move objects at the `End iterator` position that needs to removed. This will remove everything that satisfies the condition
        orderedObjList.erase(it, orderedObjList.end()); //remove all the objects starting with it to end. Read above comment firstly
    }

    //NOTE: NOT USED YET. For now I am unsure how this function should be. Game objects always constructed as unique_ptr. Removing them from container will already deallocate the game object. So
    //UnregisterGameObject function will be enough to remove the object from the container (for now).
    template <typename T>
    void DestroyGameObject(std::unique_ptr<T>& obj)
    {
        if (!obj) return; // Safety check

        UnregisterGameObject(obj->SetObjectNameToSelfClassName());

        // Reset the unique_ptr to release memory
        obj.reset();
    }
}
