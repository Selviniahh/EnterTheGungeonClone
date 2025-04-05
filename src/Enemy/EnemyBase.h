#pragma once
#include "../Core/GameObjectBase.h"
#include "../Core/Components/BaseAnimComp.h"
#include "../Core/Events/EventDelegate.h"
#include "Components/EnemyMoveCompBase.h"

namespace ETG
{
    class ProjectileBase;
    class Hero;
    class Hand;
    class RogueSpecial;
    class BaseHealthComp;
    class CollisionComponent;
    class EnemyMoveCompBase;

    class EnemyBase : public GameObjectBase
    {
    protected:
        EnemyBase(); // Add default constructor
        ~EnemyBase() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;

    public:
        // Projectile collision handling
        virtual void HandleProjectileCollision(const ProjectileBase* projectile);

        // Force handling helpers that delegate to MoveComp
        void ApplyForce(const sf::Vector2f& forceDirection, float magnitude, float forceDuration) const;
        [[nodiscard]] bool IsBeingForced() const { return MoveComp ? MoveComp->IsBeingForced : false; }
        bool IsDead() const { return EnemyState == EnemyStateEnum::Die; }

        EnemyStateEnum EnemyState{EnemyStateEnum::Idle};
        Direction EnemyDir{Direction::Right};

        std::unique_ptr<CollisionComponent> CollisionComp;
        std::unique_ptr<EnemyMoveCompBase> MoveComp;
        std::unique_ptr<BaseHealthComp> HealthComp;

        float KnockBackMagnitudeDuringDeath = 75;
        float KnockBackDurationDuringDeath = 1.0f;

    protected:
        std::unique_ptr<Hand> Hand;
        Hero* Hero;

        //I am not sure is this the right way. Force should take time based on the hero's gun's fire rate. However, it also should be faster so that the enemy can stop force and have some time to shoot
        //before another projectile is fired. After some tests, I decided to divide Gun->FireRate / 3
        float ForceDurationDivider = 3.f;

        BOOST_DESCRIBE_CLASS(EnemyBase, (GameObjectBase), (Hero), (ForceDurationDivider), ())
    };
}
