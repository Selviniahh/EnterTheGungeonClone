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
    this->Position = position;
    Depth = 2; // Set depth like Hero does

    //Collision
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 10.0f;
    CollisionComp->CollisionVisualizationColor = sf::Color::Magenta;
    CollisionComp->SetCollisionEnabled(true);

    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        if (auto* heroObj = dynamic_cast<class Hero*>(eventData.Other))
        {
            // Stop movement when colliding with hero
            isInAttackRange = true;
        }
    });

    CollisionComp->OnCollisionExit.AddListener([this](const CollisionEventData& eventData)
    {
        if (auto* heroObj = dynamic_cast<class Hero*>(eventData.Other))
        {
            // Resume movement when no longer colliding with hero
            isInAttackRange = false;
        }
    });

    BulletMan::Initialize();
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
    EnemyBase::Update(); // Start with base update

    CollisionComp->Update();
    MoveComp->Update();

    // Update animation Flip sprites based on direction like Hero does
    AnimationComp->FlipSpritesX(BulletManDir, *this);
    AnimationComp->FlipSpritesY<GunBase>(BulletManDir, *Gun);
    AnimationComp->Update();

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
    Gun->Update();

    EnemyBase::Update();
}

void ETG::BulletMan::BulletManShoot()
{
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
