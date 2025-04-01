#include "BulletMan.h"
#include <filesystem>
#include "../../Core/Factory.h"
#include "../../Managers/DebugTexts.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Utils/Math.h"
#include "Components/BulletManAnimComp.h"
#include "Components/BulletManMoveComp.h"
#include "../../Core/Components/CollisionComponent.h"
#include "../../Characters/Hand/Hand.h"
#include "../../Guns/Base/GunBase.h"
#include "../../Guns/Magnum/Magnum.h"
#include "../../Projectile/ProjectileBase.h"
#include<SFML/Graphics/Texture.hpp>

namespace ETG
{
    class Magnum;
}

ETG::BulletMan::BulletMan(const sf::Vector2f& position)
{
    this->Position = position;
    Depth = 2; // Set depth like Hero does

    //Collision
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 4.f;
    CollisionComp->CollisionVisualizationColor = sf::Color::Magenta;
    CollisionComp->SetCollisionEnabled(true);

    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        // Check if we collided with a projectile and it's not from an enemy's projectile
        const auto* projectile = dynamic_cast<ProjectileBase*>(eventData.Other);
        const auto* enemyObj = dynamic_cast<EnemyBase*>(eventData.Other->Owner->Owner);
        if (projectile && !enemyObj)
        {
            HandleProjectileCollision(projectile);
        }

        // Handle hero collision if needed
        if (auto* heroObj = dynamic_cast<class Hero*>(eventData.Other))
        {
            isInAttackRange = true;
        }
    });

    CollisionComp->OnCollisionExit.AddListener([this](const CollisionEventData& eventData)
    {
        if (!eventData.Other->IsValid()) return; //In case this is a projectile just destroyed, avoid dangling pointers
        if (const auto* heroObj = dynamic_cast<class Hero*>(eventData.Other))
        {
            isInAttackRange = false;
        }
    });

    // Set up force event handlers
    OnForceStart.AddListener([this]()
    {
        BulletManState = EnemyStateEnum::Hit;
    });

    OnForceEnd.AddListener([this]()
    {
        // Reset to idle state when force ends
        if (BulletManState == EnemyStateEnum::Hit)
            BulletManState = EnemyStateEnum::Idle;
    });

    BulletMan::Initialize();
}

void ETG::BulletMan::HandleProjectileCollision(const ProjectileBase* projectile)
{
    // Check if this is a hero projectile
    GameObjectBase* projectileOwner = projectile->Owner;
    if (!projectileOwner) return;

    GameObjectBase* rootOwner = projectileOwner->Owner;
    if (!rootOwner) return;

    if (dynamic_cast<class Hero*>(rootOwner) || dynamic_cast<class Hero*>(projectileOwner))
    {
        // This is a hero projectile that hit us

        // Calculate force direction (from projectile to bulletman)
        const sf::Vector2f forceDirection = Math::Normalize(this->Position - projectile->GetPosition());

        // Get force from projectile or use a default value
        float forceMagnitude = projectile->Force;

        // Apply the force
        ApplyForce(forceDirection, forceMagnitude);
    }
}

ETG::BulletMan::~BulletMan() = default;

void ETG::BulletMan::Initialize()
{
    EnemyBase::Initialize();

    Hand = ETG::CreateGameObjectAttached<class Hand>(this);

    // Initialize animation component
    AnimationComp = ETG::CreateGameObjectAttached<BulletManAnimComp>(this);
    AnimationComp->Initialize();
    AnimationComp->Update();

    // Initialize movement component
    MoveComp = ETG::CreateGameObjectAttached<BulletManMoveComp>(this);
    MoveComp->Initialize();

    Gun = ETG::CreateGameObjectAttached<Magnum>(this, Hand->GetRelativePosition());
    Gun->Initialize();

    Gun->ProjTexture->loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Projectiles/Enemy" / "8x8_enemy_projectile_001.png").string());

    EnemyBase::Initialize();
}

void ETG::BulletMan::Update()
{
    EnemyBase::Update(); // This now includes UpdateForce()

    // Only process movement and AI if not being forced
    if (!IsBeingForced)
    {
        CollisionComp->Update();
        MoveComp->Update();

        // Update animation Flip sprites based on direction like Hero does
        AnimationComp->FlipSpritesX(BulletManDir, *this);
        AnimationComp->FlipSpritesY<GunBase>(BulletManDir, *Gun);

        //Set hand properties
        const sf::Vector2f HandOffsetForHero = AnimationComp->IsFacingRight(BulletManDir) ? sf::Vector2f{8.f, 5.f} : sf::Vector2f{-8.f, 5.f};
        Hand->SetPosition(Position + Hand->HandOffset + HandOffsetForHero);
        Hand->Update();

        //Rest is all gun shooting and gun orientation
        if (Hand && Gun)
        {
            // Position the gun relative to the hand.
            const sf::Vector2f handPos = Hand->GetPosition();
            Gun->SetPosition(handPos + Hand->GunOffset);

            // Aim the gun toward the hero.
            const float angle = Math::AngleBetween(handPos, Hero->GetPosition());
            Gun->Rotation = angle;
        }

        if (BulletManState == EnemyStateEnum::Shooting)
        {
            Gun->PrepareShooting();
        }

        //Decrement the attack timer
        if (attackCooldownTimer > 0)
        {
            attackCooldownTimer -= Globals::FrameTick;
        }
    }

    // Always update animation and gun regardless of force state
    AnimationComp->Update();
    Gun->Update();
}

void ETG::BulletMan::BulletManShoot()
{
    // Don't shoot if being forced/hit
    if (IsBeingForced) return;

    //If the gun is shooting, we have to set enemy's animation to be shooting as well
    if (Gun->CurrentGunState == GunStateEnum::Shoot && !Gun->GetAnimationInterface()->GetAnimation()->IsAnimationFinished())
    {
        BulletManState = EnemyStateEnum::Shooting;
    }

    if (attackCooldownTimer <= 0)
    {
        // In attack range and cooldown finished, enter shooting state
        BulletManState = EnemyStateEnum::Shooting;
        attackCooldownTimer = attackCooldown; // Reset cooldown
    }
}


void ETG::BulletMan::Draw()
{
    if (!IsVisible) return;
    EnemyBase::Draw();
    SpriteBatch::Draw(GetDrawProperties());
    if (CollisionComp) CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
    Hand->Draw();
    Gun->Draw();
}
