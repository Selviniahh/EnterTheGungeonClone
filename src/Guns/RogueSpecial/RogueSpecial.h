#pragma once
#include "../Base/GunBase.h"

namespace ETG
{
    class RogueSpecial : public GunBase
    {
    public:
        explicit RogueSpecial(const sf::Vector2f& Position);
        ~RogueSpecial() override = default;

        void Initialize() override;
    };

    class RogueSpecialAnimComp : public GunAnimComp
    {
    public:
        RogueSpecialAnimComp();
        void SetAnimations() override;
    };
}