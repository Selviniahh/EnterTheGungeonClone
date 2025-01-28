#pragma once
#include <utility>

namespace ETG
{
    struct PairHash {
        template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2>& pair) const {
            auto hash1 = std::hash<T1>{}(pair.first);
            auto hash2 = std::hash<T2>{}(pair.second);
            return hash1 ^ (hash2 << 1);
        }
    };
    
    class Misc
    {
    public:
        static float RadiansToDegrees(float radians);

    private:
        //Private constructor to prevent instantiation
        Misc() = delete;
    };
}
