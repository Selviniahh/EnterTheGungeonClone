#pragma once

namespace ETG
{
    class Hero;
    
    class MoveComponent
    {
    public:
        float Speed{100.f};
        MoveComponent();
        void UpdateMovement(Hero& hero) const;
    };
}
