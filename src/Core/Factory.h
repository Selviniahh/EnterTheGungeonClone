#include <memory>
#include "../Managers/GameState.h"
#include "../Core/GameObject.h"

//Factory global functions: 
namespace ETG
{
    //NOTE: So important. Implemented Factory Method.
    //It's required to first construct the object afterwards, call some functions automatically for all game objects. Calling stuffs in Constructor of the base class will not be applicable for RTTI
    //Because RTTI will only retain base class' metadata. Employing factory method will let me set object name easily.   
    template <typename T, typename... Args>
    std::unique_ptr<T> CreateGameObject(Args&&... args)
    {
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);

        // After full construction, set the object name based on its true dynamic type.
        obj->SetObjectNameToSelfClassName();
        return obj;
    }

    template<typename T>
    void DestroyGameObject(std::unique_ptr<T>& obj)
    {
        auto& scene = GameState::GetInstance().GetSceneObj();

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
