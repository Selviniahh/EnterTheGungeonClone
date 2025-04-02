#include "EnemyBase.h"
#include "../Utils/Math.h"
#include "../Characters/Hand/Hand.h"
#include "../Characters/Hero.h"
#include "../Core/Components/CollisionComponent.h"
#include "../Managers/Globals.h"
#include "../Core/Factory.h"
#include "../Projectile/ProjectileBase.h"

namespace ETG
{
    EnemyBase::EnemyBase() : Hand(nullptr), Hero(GameState::GetInstance().GetHero()) //HealthComp(nullptr),
    {
        EnemyBase::Initialize();
    }

    EnemyBase::~EnemyBase() = default;

    void EnemyBase::Initialize()
    {
        CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
        CollisionComp->CollisionRadius = 4.f;
        CollisionComp->CollisionVisualizationColor = sf::Color::Magenta;
        CollisionComp->SetCollisionEnabled(true);

        CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
        {
            // Check if we collided with a projectile
            const auto* projectile = dynamic_cast<ProjectileBase*>(eventData.Other);
            if (projectile)
            {
                HandleProjectileCollision(projectile);
            }
        });
    }

    void EnemyBase::Update()
    {
        // Update force effect
        UpdateForce();

        GameObjectBase::Update();
    }

    void EnemyBase::Draw()
    {
        // Implementation
        GameObjectBase::Draw();
    }

    void EnemyBase::ApplyForce(const sf::Vector2f& forceDirection, float magnitude, float forceDuration)
    {
        // Set force parameters
        ForceDirection = forceDirection;
        ForceMagnitude = magnitude;
        ForceMaxDuration = forceDuration;
        ForceTimer = 0.0f;
        IsBeingForced = true;

        // Broadcast force start event
        OnForceStart.Broadcast();
    }

    void EnemyBase::UpdateForce()
    {
        if (!IsBeingForced) return;

        // Increment force timer
        ForceTimer += Globals::FrameTick;

        if (ForceTimer < ForceMaxDuration)
        {
            // Calculate current force magnitude using lerp (starts strong, gradually weakens)
            const float currentForce = Math::IntervalLerp(ForceMagnitude * ForceMultiply, 0.0f, ForceMaxDuration, ForceTimer);

            // Apply force to position
            Position += ForceDirection * currentForce * Globals::FrameTick;
        }
        else
        {
            // Force effect is done
            IsBeingForced = false;

            // Broadcast force end event
            OnForceEnd.Broadcast();
        }
    }

    void EnemyBase::HandleProjectileCollision(const ProjectileBase* projectile)
    {
        // Check if this is a hero projectile
        auto projectileOwnerGun = dynamic_cast<GunBase*>(projectile->Owner);
        if (!projectileOwnerGun) return;

        GameObjectBase* rootOwner = projectileOwnerGun->Owner;
        if (!rootOwner) return;

        if (dynamic_cast<class Hero*>(rootOwner) || dynamic_cast<class Hero*>(projectileOwnerGun))
        {
            // This is a hero projectile that hit us

            // Calculate force direction (from projectile to bulletman)
            const sf::Vector2f forceDirection = Math::Normalize(this->Position - projectile->GetPosition());

            // Get force from projectile or use a default value
            float forceMagnitude = projectile->Force;

            // Apply the force
            ApplyForce(forceDirection, forceMagnitude, (projectileOwnerGun->FireRate / 3.0));
        }
    }
}
