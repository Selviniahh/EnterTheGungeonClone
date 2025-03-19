#pragma once
#include "../EnemyBase.h"
#include "../../Characters/Hero.h"

namespace ETG
{
    class BulletManAnimComp;

    class BulletMan : public EnemyBase
    {
    public:
        explicit BulletMan(const sf::Vector2f& position);
        ~BulletMan() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;


        EnemyStateEnum BulletManState{EnemyStateEnum::Idle}; // Initialize with default state
        Direction BulletManDir{Direction::Right}; // Initialize with default direction

        std::unique_ptr<BulletManAnimComp> AnimationComp;
        std::unique_ptr<CollisionComponent> CollisionComp;

    };
}
