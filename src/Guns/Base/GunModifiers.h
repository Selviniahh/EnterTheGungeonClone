#pragma once
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

namespace ETG
{
    class GunModifier
    {
    public:
        virtual ~GunModifier() = default;
        virtual std::string GetModifierName() const = 0;
    };
    
    //-------------------------------------Gun Modifier--------------------------------------
    class GunModifierManager
    {
    public:
        //Add a modifier to the gun
        void AddModifier(const std::shared_ptr<GunModifier>& modifier)
        {
            //Remove any existing modifier of same type
            RemoveModifier(modifier->GetModifierName());
            modifiers.push_back(modifier);
        }

        //Remove a modifier by name
        void RemoveModifier(const std::string& modifierName)
        {
            //Better and cooler way to remove from an array instead of traditional for loop. In traditional loop, when I need to remove multiple elements, it crashes
            //So I had to cache first the elements needs to be removed in loop, after the loop ends, then I'd remove the elements with using the cache. But this erase-remove idiom completely fixed everythng
            modifiers.erase(
                std::ranges::remove_if(modifiers,
                                       [&](const std::shared_ptr<GunModifier>& mod)
                                       {
                                           return mod->GetModifierName() == modifierName;
                                       }).begin(),
                modifiers.end());
        }

        //Get a specific type of modifier with same T 
        template <typename T>
        std::shared_ptr<T> GetModifier() const
        {
            for (const auto& mod : modifiers)
            {
                if (auto casted = std::dynamic_pointer_cast<T>(mod)) //attempts to cast each mod to a shared_ptr<T>
                    return casted;
            }
            return nullptr;
        }

        void ClearAllModifiers()
        {
            modifiers.clear();
        }

        //Check if a specific modifier exists
        bool HasModifier(const std::string& modifierName) const
        {
            return std::ranges::any_of(modifiers, [&](const auto& mod)
            {
               return mod->GetModifierName() == modifierName; 
            });
        }

        //Get number of active modifiers
        size_t GetModifierCount() const
        {
            return modifiers.size();
        }

    private:
        std::vector<std::shared_ptr<GunModifier>> modifiers;
    };
}
