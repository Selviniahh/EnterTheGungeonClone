#pragma once
#include <functional>
#include <string>

#include "../../src/Managers/StateEnums.h"

namespace ETG
{
    enum class Direction;

    struct PairHash
    {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const
        {
            auto hash1 = std::hash<T1>{}(pair.first);
            auto hash2 = std::hash<T2>{}(pair.second);
            return hash1 ^ (hash2 << 1);
        }
    };

    class DirectionUtils
    {
    public:
        //Pair contains direction range. Value provides the corresponding Direction for the given pair range.  
        using DirectionMap = std::unordered_map<std::pair<int, int>, Direction, PairHash>&;
        
        // Populates the map with default angle→direction ranges
        static void PopulateDirectionRanges(DirectionMap mapToFill);

        //DirectionMap's key pair represents minimum and maximum degree range. The value is the corresponding Direction for degree range.
        //In Short, take the map and calculated angle, and return the Found Direction from angle. 
        static Direction GetDirectionFromAngle(const std::unordered_map<std::pair<int, int>, Direction, PairHash>& DirectionMap, float angle);
        
        static IdleEnum GetIdleDirectionEnum(Direction currDir);
        static RunEnum GetRunEnum(Direction currDir);

        static std::string StringifyDirection(Direction dir);
    };
    
}
