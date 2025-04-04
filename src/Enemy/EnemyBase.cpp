#include "EnemyBase.h"
#include "../Utils/Math.h"
#include "../Characters/Hand/Hand.h"
#include "../Characters/Hero.h"
#include "../Core/Components/CollisionComponent.h"
#include "../Core/Components/BaseHealthComp.h"
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

        OnShooting.AddListener([this]()
        {
            HandleShooting();
        });
    }

    EnemyBase::~EnemyBase() = default;

    void EnemyBase::Initialize()
    {
        CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
        CollisionComp->CollisionRadius = 4.f;
        CollisionComp->CollisionVisualizationColor = sf::Color::Magenta;
        CollisionComp->SetCollisionEnabled(true);

        HealthComp = ETG::CreateGameObjectAttached<BaseHealthComp>(this, 50.f); //Health is 50
        HealthComp->OnDamageTaken.AddListener([this](const float damage,  GameObjectBase* instigator)   
        {
            //Enemy took damage
            //NOTE: Shall we make instigator ProjectileBase? but later on enemy should also get damage from non projectile stuffs like explosive barrels and other things
            EnemyState = EnemyStateEnum::Hit;
            HandleProjectileCollision(dynamic_cast<const ProjectileBase*>(instigator));
        });

        MoveComp = ETG::CreateGameObjectAttached<EnemyMoveCompBase>(this);
        MoveComp->Initialize();

        CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
        {
            // Check if we collided with a projectile and it's fired from hero
            auto* projectile = dynamic_cast<ProjectileBase*>(eventData.Other);
            const auto* hero = dynamic_cast<class Hero*>(eventData.Other->Owner->Owner);
            if (projectile && hero)
            {
                HealthComp->ApplyDamage(projectile->Damage, projectile);
            }
        });
    }

    void EnemyBase::Update()
    {
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
        if (!projectileOwnerGun) return;

        GameObjectBase* GunOwner = projectileOwnerGun->Owner;
        if (!GunOwner) return;

        // is this a hero projectile that hit us
        if (dynamic_cast<class Hero*>(GunOwner))
        {
            // Calculate force direction (from projectile to enemy)
            const sf::Vector2f forceDirection = Math::Normalize(this->Position - projectile->GetPosition());

            // Get force from projectile
            const float forceMagnitude = projectile->Force;

            // Apply the force
            MoveComp->ApplyForce(forceDirection, forceMagnitude, projectileOwnerGun->FireRate / ForceDurationDivider);
        }
    }

    void EnemyBase::HandleShooting()
    {
        // Don't shoot if being forced/hit
        if (MoveComp->IsBeingForced) return;

        //If the gun is shooting, and it's shoot animation is not over, we have to set enemy's animation to be shooting as well
        if (Gun->CurrentGunState == GunStateEnum::Shoot && !Gun->GetAnimationInterface()->GetAnimation()->IsAnimationFinished())
        {
            EnemyState = EnemyStateEnum::Shooting;
        }

        // If AttackCooldownTimer is up, SHOOT!
        if (attackCooldownTimer <= 0)
        {
            EnemyState = EnemyStateEnum::Shooting;
            attackCooldownTimer = attackCooldown; // Reset cooldown
        }
    }
}
