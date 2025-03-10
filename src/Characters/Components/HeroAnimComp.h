#pragma once
#include <SFML/Graphics.hpp>
#include "../../Core/Components/BaseAnimComp.h"
#include "../../Characters/Hero.h"

namespace ETG
{
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

    template <typename ... TObjects>
    sf::Vector2f HeroAnimComp::FlipSprites(const Direction& currentDirection, TObjects&... objects)
    {
        if (!AnimManagerDict.contains(CurrentState)) return {8.f, 5.f};

        const bool facingRight =
            currentDirection == Direction::Right || currentDirection == Direction::FrontHandRight ||
            currentDirection == Direction::BackDiagonalRight || currentDirection == Direction::BackHandRight;

        //set horizontal scale to face the correct direction. Note that if abs not given, it will forever flip other direction in every tick 
        HeroPtr->SetScale({
            facingRight ? std::abs(HeroPtr->GetScale().x) : -std::abs(HeroPtr->GetScale().x),
            HeroPtr->GetScale().y
        });

        //lambda that applies the scale flip to an object. It'll only flip gun on Y axis according to hero's direction for now. 
        auto flipObjectScale = [facingRight](auto& obj)
        {
            // obj.GetScale().y = flipFactor;
            obj.SetScale({
                obj.GetScale().x,
                facingRight ? std::abs(obj.GetScale().y) : -std::abs(obj.GetScale().y)
            });
        };

        //Apply the flip to all passed objects
        (flipObjectScale(objects), ...);

        return facingRight ? sf::Vector2f{8.f, 5.f} : sf::Vector2f{-7.f, 5.f};
    }
}
