#pragma once
#include "../Core/GameObjectBase.h"
#include "../Core/Components/BaseAnimComp.h"
#include "../Core/Events/EventDelegate.h"
#include "Components/EnemyMoveCompBase.h"

namespace ETG
{
    class Magnum;
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
        bool IsBeingForced() const { return MoveComp ? MoveComp->IsBeingForced : false; }
        virtual void EnemyShoot(); //Shoot if timer is up and attackDistance

        EnemyStateEnum EnemyState{EnemyStateEnum::Idle};
        Direction EnemyDir{Direction::Right};

        // Attack parameters...
        float attackCooldown = 2.0f;
        float attackCooldownTimer = 0.0f;
        bool isInAttackRange = false; //For now 

        std::unique_ptr<Magnum> Gun; //TODO: MAke this GunBase after you're done with all

        std::unique_ptr<CollisionComponent> CollisionComp;
        std::unique_ptr<EnemyMoveCompBase> MoveComp;

    protected:
        std::unique_ptr<Hand> Hand;
        Hero* Hero;

        //I am not sure is this the right way. Force should take time based on the hero's gun's fire rate. However, it also should be faster so that the enemy can stop force and have some time to shoot
        //before another projectile is fired. After some tests, I decided to divide Gun->FireRate / 3
        float ForceDurationDivider = 3.f; 

        BOOST_DESCRIBE_CLASS(EnemyBase, (GameObjectBase), (Hero), (ForceDurationDivider), ())
    };
}
