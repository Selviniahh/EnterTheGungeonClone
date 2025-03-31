#pragma once
#include "../EnemyBase.h"
#include "../../Characters/Hero.h"

namespace ETG
{
    class BulletManAnimComp;
    class BulletManMoveComp;

    class BulletMan : public EnemyBase
    {
    public:
        explicit BulletMan(const sf::Vector2f& position);
        ~BulletMan() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;

        EnemyStateEnum BulletManState{EnemyStateEnum::Idle};\
        Direction BulletManDir{Direction::Right};

        std::unique_ptr<BulletManAnimComp> AnimationComp;
        std::unique_ptr<BulletManMoveComp> MoveComp;
        std::unique_ptr<CollisionComponent> CollisionComp;
        
        // Attack parameters
        float attackDistance = 100.0f;   // Distance from which attack is possible
        float attackCooldown = 2.0f;     // Time between attacks (seconds)
        float attackCooldownTimer = 0.0f; // Current attack cooldown timer
        
        bool isInAttackRange = false;    // Set to true when colliding with hero

        BOOST_DESCRIBE_CLASS(BulletMan, (EnemyBase),
                              (BulletManState, BulletManDir, attackDistance, attackCooldown, attackCooldownTimer, isInAttackRange),
                              (), ())
    };
}