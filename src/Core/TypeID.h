#pragma once
#include <cstdint>
#include <string_view>
#include <unordered_set>
#include <typeindex>
#include <unordered_map>

namespace ETG
{
    class TypeID
    {
    public:
        using IDType = uint32_t;

        // Get unique ID for a type
        template<typename T>
        static IDType GetID() {
            static const IDType id = NextID();
            return id;
        }

        // Register inheritance relationship
        template<typename Derived, typename Base>
        static void RegisterBaseClass() {
            const IDType derivedID = GetID<Derived>();
            const IDType baseID = GetID<Base>();
            
            // Store this inheritance relationship
            BaseClasses[derivedID].insert(baseID);
        }
        
        // Check if Child inherits from Parent
        static bool IsBaseOf(IDType childID, IDType parentID) {
            if (childID == parentID) return true; // Same type
            
            // Check direct inheritance
            auto it = BaseClasses.find(childID);
            if (it != BaseClasses.end()) {
                if (it->second.contains(parentID)) return true;
                
                // Check recursive inheritance
                for (IDType baseID : it->second) {
                    if (IsBaseOf(baseID, parentID)) return true;
                }
            }
            
            return false;
        }
        
        // Get type name for debugging
        template<typename T>
        static std::string_view GetName() {
            return typeid(T).name();
        }

    private:
        static IDType NextID() {
            static IDType nextID = 0;
            return nextID++;
        }
        
        // Store inheritance relationships (derived -> set of bases)
        inline static std::unordered_map<IDType, std::unordered_set<IDType>> BaseClasses;
    };
}
