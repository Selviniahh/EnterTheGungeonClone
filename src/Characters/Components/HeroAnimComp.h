#pragma once
#include <SFML/Graphics.hpp>
#include "../../Core/Components/BaseAnimComp.h"

namespace ETG
{
    class Hero;

    class HeroAnimComp : public BaseAnimComp<HeroStateEnum>
    {
    public:
        HeroAnimComp();

        //Override
        void Update() override;
        void SetAnimations() override;

        template <typename... TObjects>
        sf::Vector2f FlipSprites(const Direction& currentDirection, TObjects&... objects);

    private:
        Hero* HeroPtr = nullptr;

        BOOST_DESCRIBE_CLASS(HeroAnimComp, (BaseAnimComp), (HeroPtr), (), ())
    };
}
