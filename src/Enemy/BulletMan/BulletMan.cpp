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
#include<SFML/Graphics/Texture.hpp>

namespace ETG
{
    class Magnum;
}

ETG::BulletMan::BulletMan(const sf::Vector2f& position)
{
    BulletMan::Initialize();

    this->Position = position;
    Depth = 2; // Set depth like Hero does

    CollisionComp->OnCollisionExit.AddListener([this](const CollisionEventData& eventData)
    {
        if (!eventData.Other->IsValid()) return; //In case this is a projectile just destroyed, avoid dangling pointers
        if (const auto* heroObj = dynamic_cast<class Hero*>(eventData.Other))
        {
            isInAttackRange = false;
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
    if (true)
    {
        CollisionComp->Update();

        //Make movement if it's not being forced 
        if (!IsBeingForced())
            MoveComp->Update();

        // Update animation Flip sprites based on direction like Hero does
        AnimationComp->FlipSpritesX(EnemyDir, *this);
        AnimationComp->FlipSpritesY<GunBase>(EnemyDir, *Gun);

        //Set hand properties
        const sf::Vector2f HandOffsetForHero = AnimationComp->IsFacingRight(EnemyDir) ? sf::Vector2f{8.f, 5.f} : sf::Vector2f{-8.f, 5.f};
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

        if (EnemyState == EnemyStateEnum::Shooting)
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
    if (IsBeingForced()) return;

    //If the gun is shooting, we have to set enemy's animation to be shooting as well
    if (Gun->CurrentGunState == GunStateEnum::Shoot && !Gun->GetAnimationInterface()->GetAnimation()->IsAnimationFinished())
    {
        EnemyState = EnemyStateEnum::Shooting;
    }

    if (attackCooldownTimer <= 0)
    {
        // In attack range and cooldown finished, enter shooting state
        EnemyState = EnemyStateEnum::Shooting;
        attackCooldownTimer = attackCooldown; // Reset cooldown
    }
}

void ETG::BulletMan::HandleProjectileCollision(const ProjectileBase* projectile)
{
    EnemyBase::HandleProjectileCollision(projectile);
    EnemyState = EnemyStateEnum::Hit;
}


void ETG::BulletMan::Draw()
{
    if (!IsVisible) return;
    EnemyBase::Draw();
    SpriteBatch::Draw(GetDrawProperties());
    if (CollisionComp) CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());

    Gun->Draw();
    Hand->Draw();
}
