#pragma once
#include "../Core/GameObjectBase.h"

namespace ETG
{
    class CollisionComponent;
    class TimerComponent;

    class ProjectileBase : public GameObjectBase
    {
    public:
        ProjectileBase() = default;
        ~ProjectileBase() override;
        ProjectileBase(const sf::Texture& texture, sf::Vector2f spawnPos, sf::Vector2f velocity, float range, float rotation, float damage = 1.f, float force = 1.f);

        void Initialize() override;
        void Update() override;
        void Draw() override;

        sf::Vector2f ProjVelocity;
        float Range;
        float Damage;
        float Force; //knockback amount

        std::unique_ptr<CollisionComponent> CollisionComp;
        std::unique_ptr<TimerComponent> TimerComp;

    private:
        float DistanceTraveled = 0.0f; //Track the total distance traveled

        BOOST_DESCRIBE_CLASS(ProjectileBase, (GameObjectBase), (ProjVelocity, Range, Damage, Force), (), ())
    };
}
