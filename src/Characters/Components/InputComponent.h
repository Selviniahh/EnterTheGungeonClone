#pragma once
#include <unordered_map>
#include "../../Utils/DirectionUtils.h"

namespace ETG
{
    struct PairHash;
    enum class Direction;
    enum class DashEnum;
}

namespace ETG
{
    class Hero;
}

namespace ETG
{
    class InputComponent
    {
    public:
        InputComponent();

        void Update(Hero& hero) const;

        static DashEnum GetDashDirectionEnum();

    private:
        std::unordered_map<std::pair<int, int>, Direction, PairHash> DirectionMap{};
        void UpdateDirection(Hero& hero) const;
    };
}
