#pragma once
#include "../Core/GameObjectBase.h"
#include "../Core/Components/BaseAnimComp.h"
#include "../Managers/StateEnums.h"
#include "../Core/Events/EventDelegate.h"

// Forward declarations


namespace ETG
{
    class ProjectileBase;
    class Hero;
    class Hand;
    class RogueSpecial;
    class BaseHealthComp;
    class CollisionComponent;

    class EnemyBase : public GameObjectBase
    {
    protected:
        EnemyBase(); // Add default constructor
        ~EnemyBase() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;

    public:
        // Force handling when hit by projectiles
        void ApplyForce(const sf::Vector2f& forceDirection, float magnitude, float forceDuration);
        void UpdateForce();
        virtual void HandleProjectileCollision(const ProjectileBase* projectile);

        std::unique_ptr<CollisionComponent> CollisionComp;
        
        // Force parameters
        float ForceMultiply = 5.0f; //NOTE: Shouldn't this be Gun's Force? 
        float ForceMagnitude = 1.f;
        float ForceTimer = 0.0f;
        float ForceMaxDuration = 0.5f;
        sf::Vector2f ForceDirection = {0.0f, 0.0f};
        bool IsBeingForced = false;
        
        // Events for force application
        EventDelegate<> OnForceStart;
        EventDelegate<> OnForceEnd;

    protected:
        // std::unique_ptr<BaseHealthComp> HealthComp;
        std::unique_ptr<Hand> Hand;
        Hero* Hero;

        BOOST_DESCRIBE_CLASS(EnemyBase, (GameObjectBase),
            (Hero, ForceMultiply, ForceMaxDuration), (), ())
    };
}
