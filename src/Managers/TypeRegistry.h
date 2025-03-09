#pragma once
#include <functional>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <utility>
#include "../Core/GameObjectBase.h"
#include "../UI/UserInterface.h"
#include "../Characters/Hero.h"
#include "../Core/Scene/Scene.h"
#include "../Characters/Components/InputComponent.h"
#include "../Core/Components/BaseMoveComp.h"
#include "../Engine/Reflection.h"
#include "../Characters/Components/HeroMoveComp.h"
#include "../Characters/Components/HeroAnimComp.h"

namespace ETG
{
    class TypeRegistry
    {
    public:
        // Register a specific type for reflection
        template <typename T>
        static void RegisterType()
        {
            // Store the type info for later lookup
            RegisteredTypes[std::type_index(typeid(T))] = TypeData{
                std::type_index(typeid(T)),
                [](GameObjectBase* obj)
                {
                    static_assert(std::is_base_of_v<GameObjectBase, T>, "The object T is not child of GameObjectBase");

                    // Convert obj to be the type of T. I know T is child of GameObjectBase
                    if (auto* Child = dynamic_cast<T*>(obj))
                    {
                        Reflection::PopulateReflection<T>(*Child);
                        return true;
                    }

                    return false;
                }
            };
        }

        //Process an object using its runtime type
        static bool ProcessObject(GameObjectBase* obj)
        {
            if (!obj)
                return false; // Changed from throwing exception to returning false

            // Try direct match first (fastest)
            const auto exactType = std::type_index(typeid(*obj));
            const auto it = RegisteredTypes.find(exactType);
            if (it != RegisteredTypes.end()) //if found
            {
                //After this line executed, if above handler function not delegated, there's a problem with given obj's type. Make sure it's only GameObject not any child of it.
                //Before calling ProcessObject, upcast with dynamic_cast
                return it->second.handler(obj); 
            }

            //Default to base class if no match
            Reflection::PopulateReflection<GameObjectBase>(*obj);
            return true;
        }

        struct TypeData
        {
            std::type_index typeIdx;
            std::function<bool(GameObjectBase*)> handler;

            TypeData() : typeIdx(typeid(void)), handler(nullptr)
            {
            }

            TypeData(const std::type_index idx, std::function<bool(GameObjectBase*)> h) : typeIdx(idx), handler(std::move(h))
            {
            }
        };

        static void InitializeTypeRegistry()
        {
            //It doesn't matter to add or not the base class if child is added
            RegisterType<GameObjectBase>(); 
            RegisterType<ComponentBase>();
            RegisterType<Scene>();
            RegisterType<Hero>();
            RegisterType<UserInterface>();
            RegisterType<InputComponent>();
            RegisterType<HeroMoveComp>();
            // RegisterType<BaseAnimComp<HeroStateEnum>>();
            RegisterType<HeroAnimComp>();
        }

        static inline std::unordered_map<std::type_index, TypeData> RegisteredTypes;
    };
}
