#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "../Core/GameObjectBase.h"
#include "../Managers/StateEnums.h"
#include "../Core/Factory.h"

namespace ETG
{
    class RogueSpecial;
    class HeroAnimComp;
    class InputComponent;
    class HeroMoveComp;
    
    class Hero : public GameObjectBase
    {
    public:
        static float MouseAngle;
        static Direction CurrentDirection;
        static bool IsShooting;

        explicit Hero(sf::Vector2f Position);
        ~Hero() override;
        void Update() override;
        void Draw() override;

        HeroStateEnum CurrentHeroState{HeroStateEnum::Idle};
        bool IsDashing = false;

        std::unique_ptr<RogueSpecial> RogueSpecial;
        std::unique_ptr<HeroMoveComp> MoveComp;

    private:
        std::unique_ptr<HeroAnimComp> AnimationComp;
        std::unique_ptr<InputComponent> InputComp;

        std::shared_ptr<sf::Texture> HandTex;
        sf::Vector2f HandPos;
        sf::Vector2f RelativeHandLoc{8, 4};
        sf::Vector2f RelativeGunOffsetPos{2,2};


        void SetHandTexLoc();
    };
}
