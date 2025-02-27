#pragma once
#include <memory>
#include "../Managers/GameState.h"

//Factory global functions: 
namespace ETG
{
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
        obj->Owner =  GameState::GetInstance().GetSceneObj();
       
        GameState::GetInstance().GetSceneObjs()[obj->SetObjectNameToSelfClassName()] = obj.get();
        return obj;
    }

    template <typename T, typename... Args>
    std::unique_ptr<T> CreateGameObjectAttached(GameObject* OwnerObj, Args&&... args)
    {
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        obj->Owner = OwnerObj;

        // After full construction, set the object name based on its true dynamic type name and Set obj to unordered_map 
        GameState::GetInstance().GetSceneObjs()[obj->SetObjectNameToSelfClassName()] = obj.get();
        return obj;
    }

    template <typename T>
    void DestroyGameObject(std::unique_ptr<T>& obj)
    {
        auto& scene = GameState::GetInstance().GetSceneObjs();

        //Remove the pointer from the scene container
        auto it = std::find(scene.begin(), scene.end(), obj.get());
        if (it != scene.end())
        {
            scene.erase(it);
        }
        else
        {
            throw std::runtime_error("The object: not found");
        }

        obj.reset();
    }
}
