#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "../Core/GameObject.h"
#include "../Guns/RogueSpecial/RogueSpecial.h"
#include "Components/HeroAnimComp.h"
#include "../Managers/StateEnums.h"
#include "Components/HeroMoveComp.h"
#include "Components/InputComponent.h"

namespace ETG
{
    class Hero : public GameObject<Hero>
    {
    public:
        static float MouseAngle;
        static Direction CurrentDirection;
        static bool IsShooting;

        explicit Hero(sf::Vector2f Position);
        void Update() override;
        void Draw() override;
        void DrawHeroBounds() const;
        sf::FloatRect HeroBounds() const;

        HeroStateEnum CurrentHeroState{HeroStateEnum::Idle};
        bool IsDashing = false;

        std::unique_ptr<RogueSpecial> RogueSpecial;
        HeroMoveComp MoveComp;

    private:
        std::unique_ptr<HeroAnimComp> AnimationComp;
        InputComponent InputComp;

        sf::Texture HandTex;
        sf::Vector2f HandPos;
        sf::Vector2f RelativeHandLoc{8, 4};
        sf::Vector2f RelativeGunOffsetPos;


        void SetHandTexLoc();
    };
}
