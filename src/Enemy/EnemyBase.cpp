#include "EnemyBase.h"
#include "../Utils/Math.h"
#include "../Characters/Hand/Hand.h"
#include "../Characters/Hero.h"
#include "../Core/Components/CollisionComponent.h"
#include "../Managers/Globals.h"
#include "../Core/Factory.h"
#include "../Projectile/ProjectileBase.h"
#include "Components/EnemyMoveCompBase.h"
#include "../Guns/Base/GunBase.h"

namespace ETG
{
    EnemyBase::EnemyBase() : Hand(nullptr), Hero(GameState::GetInstance().GetHero())
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

        MoveComp = ETG::CreateGameObjectAttached<EnemyMoveCompBase>(this);
        MoveComp->Initialize();

        CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
        {
            // Check if we collided with a projectile
            if (const auto* projectile = dynamic_cast<ProjectileBase*>(eventData.Other))
            {
                HandleProjectileCollision(projectile);
            }
        });
    }

    void EnemyBase::Update()
    {
        if (MoveComp)
            MoveComp->Update();

        GameObjectBase::Update();
    }

    void EnemyBase::Draw()
    {
        GameObjectBase::Draw();
    }

    void EnemyBase::HandleProjectileCollision(const ProjectileBase* projectile)
    {
        // Check if this is a hero projectile
        const auto projectileOwnerGun = dynamic_cast<GunBase*>(projectile->Owner);
        GameObjectBase* rootOwner = projectileOwnerGun->Owner; //Projectile's owner is a gun, gun's owner can be either another enemy or hero
        if (!rootOwner) return;

        // is this a hero projectile that hit us
        if (dynamic_cast<class Hero*>(rootOwner))
        {
            // Calculate force direction (from projectile to enemy)
            const sf::Vector2f forceDirection = Math::Normalize(this->Position - projectile->GetPosition());

            // Get force from projectile
            const float forceMagnitude = projectile->Force;

            // Apply the force
            ApplyForce(forceDirection, forceMagnitude, projectileOwnerGun->FireRate / ForceDurationDivider);
        }
    }

    void EnemyBase::ApplyForce(const sf::Vector2f& forceDirection, const float magnitude, const float forceDuration) const
    {
        MoveComp->ApplyForce(forceDirection, magnitude, forceDuration);
    }
}
