#pragma once
#include "../../../Core/Components/BaseMoveComp.h"

namespace ETG
{
    class BulletMan;

    class BulletManMoveComp : public BaseMoveComp
    {
    public:
        BulletManMoveComp();
        ~BulletManMoveComp() override = default;

        void Update() override;
        void Initialize() override;
        void UpdateMovement();

        // Movement parameters
        float DetectionRadius = 200.0f;     // How far BulletMan can detect the Hero
        float StopDistance = 100.0f;         // How close BulletMan gets before stopping
        float MovementSpeed = 40.0f;       // Speed of movement
        
        BulletMan* BulletManPtr = nullptr;

    private:
        bool IsHeroInRange() const;
        
        BOOST_DESCRIBE_CLASS(BulletManMoveComp, (BaseMoveComp),
                             (BulletManPtr, DetectionRadius, StopDistance, MovementSpeed), (), ())
    };
}