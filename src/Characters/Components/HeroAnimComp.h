#pragma once
#include <SFML/Graphics.hpp>
#include "../../Managers/StateEnums.h"
#include "../../Core/Components/BaseAnimComp.h"
namespace ETG
{
    enum class HeroStateEnum;
    class Hero;

    class HeroAnimComp : public BaseAnimComp<HeroStateEnum>
    {
    public:
        HeroAnimComp();

        //Override
        void Update() override;
        void SetAnimations() override;
        
        template<typename... TObjects>
         sf::Vector2f FlipSprites(const Direction& currentDirection, TObjects&... objects);

    private:
        Hero* HeroPtr = nullptr;
    };
}
