#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

#include "../Core/GameObject.h"
#include "../Guns/Base/GunBase.h"
#include "Components/AnimationComponent.h"
#include "../Managers/StateEnums.h"
#include "Components/MoveComponent.h"
#include "Components/InputComponent.h"

namespace ETG
{
    class Hero : public GameObject
    {
    public:
        static sf::Vector2f HeroPosition;
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

        GunBase Gun;

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
