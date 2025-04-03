#pragma once
#include "../EnemyBase.h"
#include "../../Characters/Hero.h"

namespace ETG
{
    class BulletManAnimComp;
    class Magnum;
    class ProjectileBase;

    class BulletMan : public EnemyBase
    {
    public:
        explicit BulletMan(const sf::Vector2f& position);
        ~BulletMan() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;
        void HandleProjectileCollision(const ProjectileBase* projectile) override;

        std::unique_ptr<BulletManAnimComp> AnimationComp;

        BOOST_DESCRIBE_CLASS(BulletMan, (EnemyBase),
                             (),
                             (), ())
    };
}
