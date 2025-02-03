#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "Components/AnimationComponent.h"
#include "../Managers/StateEnums.h"
#include "Components/MoveComponent.h"
#include "Components/InputComponent.h"

namespace ETG
{
}

namespace ETG
{
    class Hero
    {
    public:
        static sf::Vector2f HeroPosition;
        static float MouseAngle;
        static Direction CurrentDirection;

        explicit Hero(sf::Vector2f Position);
        void Update();
        void Draw();
        void DrawHeroBounds() const;
        sf::FloatRect HeroBounds() const;

        HeroStateEnum CurrentHeroState{HeroStateEnum::Idle};
        bool IsDashing = false;

    private:
        AnimationComponent AnimationComp;
        InputComponent InputComp;
        MoveComponent MoveComp;

        sf::Texture HandTex;
        sf::Vector2f HandPos;
        sf::Vector2f RelativeHandLoc{8, 4};

        static IdleEnum GetIdleDirectionEnum(Direction currDir);
        static RunEnum GetRunEnum(Direction currDir);
        void SetHandTexLoc();
    };
}
