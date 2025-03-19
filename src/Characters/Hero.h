#pragma once
#include <SFML/System/Vector2.hpp>
#include "../Core/GameObjectBase.h"
#include "../Managers/StateEnums.h"
#include "../Core/Factory.h"

namespace ETG
{
    class CollisionComponent;
}

namespace ETG
{
    class ReloadText;
    class GunBase;
    class Hand;
    class RogueSpecial;
    class HeroAnimComp;
    class InputComponent;
    class HeroMoveComp;
    class ReloadSlider;

    
    class Hero : public GameObjectBase
    {
    public:
        explicit Hero(sf::Vector2f Position);
        ~Hero() override;
        void Update() override;
        void Initialize() override;
        void Draw() override;
        [[nodiscard]] GunBase* GetCurrentHoldingGun() const;

        static float MouseAngle;
        static Direction CurrentDirection;
        static bool IsShooting;

        HeroStateEnum CurrentHeroState{HeroStateEnum::Idle};
        bool IsDashing = false;
        
        std::unique_ptr<RogueSpecial> RogueSpecial;
        std::unique_ptr<HeroMoveComp> MoveComp;
        std::unique_ptr<Hand> Hand;
        std::unique_ptr<ReloadText> ReloadText;
        std::unique_ptr<CollisionComponent> CollisionComp;


    private:
        std::unique_ptr<HeroAnimComp> AnimationComp;
        std::unique_ptr<InputComponent> InputComp;
        
        BOOST_DESCRIBE_CLASS(Hero,(GameObjectBase),
            (MouseAngle, CurrentDirection, CurrentHeroState, IsDashing, IsShooting),
            (),
            ())
    };
}
