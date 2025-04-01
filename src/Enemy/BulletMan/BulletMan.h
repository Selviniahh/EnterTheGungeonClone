#pragma once
#include "../EnemyBase.h"
#include "../../Characters/Hero.h"

namespace ETG
{
    class BulletManAnimComp;
    class BulletManMoveComp;
    class Magnum;


    class BulletMan : public EnemyBase
    {
    public:
        explicit BulletMan(const sf::Vector2f& position);
        ~BulletMan() override;
        void Initialize() override;
        void Update() override;
        void BulletManShoot(); //Shoot if timer is up and attackDistance
        void Draw() override;

        EnemyStateEnum BulletManState{EnemyStateEnum::Idle};
        Direction BulletManDir{Direction::Right};

        std::unique_ptr<BulletManAnimComp> AnimationComp;
        std::unique_ptr<BulletManMoveComp> MoveComp;
        std::unique_ptr<CollisionComponent> CollisionComp;

        // Attack parameters...
        float attackCooldown = 2.0f;
        float attackCooldownTimer = 0.0f;
        bool isInAttackRange = false; //For now 

        std::unique_ptr<Magnum> Gun;

        BOOST_DESCRIBE_CLASS(BulletMan, (EnemyBase),
                             (BulletManState, BulletManDir, attackCooldown, attackCooldownTimer, isInAttackRange),
                             (), ())
    };
}
