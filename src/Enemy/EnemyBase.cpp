#include "EnemyBase.h"
#include "../Utils/Math.h"
#include "../Characters/Hand/Hand.h"
#include "../Characters/Hero.h"
#include "../Core/Components/CollisionComponent.h"
#include "../Managers/Globals.h"
#include "../Core/Factory.h"
#include "../Core/Components/BaseHealthComp.h"
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

        HealthComp = ETG::CreateGameObjectAttached<BaseHealthComp>(this, 30.f);
        HealthComp->Initialize();

        CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
        {
            // Check if we collided with a projectile
            if (const auto projectile = dynamic_cast<ProjectileBase*>(eventData.Other))
            {
                // HandleProjectileCollision(projectile);
                HealthComp->ApplyDamage(projectile->Damage, projectile);
            }
        });
        
        // Set up force event handlers
        MoveComp->OnForceStart.AddListener([this]()
        {
            EnemyState = EnemyStateEnum::Hit;
        });

        MoveComp->OnForceEnd.AddListener([this]()
        {
            // Reset to idle state when force ends
            if (EnemyState == EnemyStateEnum::Hit)
                EnemyState = EnemyStateEnum::Idle;
        });

        HealthComp->OnDeath.AddListener([this](const GameObjectBase* instigator)
        {
            // If dead, ignore the damage
            if (EnemyState == EnemyStateEnum::Die) return;

            // Set enemy state to hit
            EnemyState = EnemyStateEnum::Die;
            const sf::Vector2f knockbackDir = Math::Normalize(Position - instigator->GetPosition());
            MoveComp->ApplyForce(knockbackDir, KnockBackMagnitudeDuringDeath, KnockBackDurationDuringDeath);

            //TODO: Clearing these delegates shooting exception. Fix this becuase I don't wanna manually add bool flags in other delegates. Simply clearing the delegate should be good
            //Clear the delegates to not let any interaction
            // MoveComp->OnForceStart.Clear();
            // MoveComp->OnForceEnd.Clear();
            // HealthComp->OnDeath.Clear();
            // HealthComp->OnDamageTaken.Clear();
            CollisionComp->SetCollisionEnabled(false);
        });

        HealthComp->OnDamageTaken.AddListener([this](const float damage, const GameObjectBase* instigator)
        {
            //IF dead ignore the damage
            if (EnemyState == EnemyStateEnum::Die) return;

            EnemyState = EnemyStateEnum::Hit;
            HandleProjectileCollision(dynamic_cast<const ProjectileBase*>(instigator));
        });
    }

    void EnemyBase::Update()
    {
        MoveComp->Update();
        HealthComp->Update();

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
