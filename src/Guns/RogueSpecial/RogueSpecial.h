#pragma once
#include "../../Core/Components/BaseAnimComp.h"
#include "../Base/GunBase.h"

namespace ETG
{
    class RogueSpecialAnimComp : public BaseAnimComp<RogueSpecialAnimComp, GunStateEnum>
    {
    public:
        RogueSpecialAnimComp();
        void SetAnimations() override;
    };
    
    class RogueSpecial : public GunBase<RogueSpecial, RogueSpecialAnimComp>
    {
    public:
        explicit RogueSpecial(const sf::Vector2f& Position);
        ~RogueSpecial() override = default;

        void Initialize() override;
    };
}