#pragma once
#include "../../Core/Components/BaseAnimComp.h"
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

    class RogueSpecialAnimComp : public BaseAnimComp<GunStateEnum>
    {
    public:
        RogueSpecialAnimComp();
        void SetAnimations() override;
    };
}