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

#include "../../Core/Components/BaseHealthComp.h"

namespace ETG
{
    class Magnum;
}

ETG::BulletMan::BulletMan(const sf::Vector2f& position)
{
    BulletMan::Initialize();

    this->Position = position;
    Depth = 4;
}

ETG::BulletMan::~BulletMan() = default;

void ETG::BulletMan::Initialize()
{
    EnemyBase::Initialize();

    //NOTE: VERY IMPORTANT. We need to reset the Move comp because in enemy `MoveComp` is EnemyBaseMoveComp. We want to replace it with BulletManMoveComp
    //Solution is create our desired variable and assign it to MoveComp
    auto newSpecializedMoveComp = ETG::CreateGameObjectDefault<BulletManMoveComp>();
    newSpecializedMoveComp->Owner = this;
    MoveComp = std::move(newSpecializedMoveComp);
    MoveComp->Initialize();
    
    Hand = ETG::CreateGameObjectAttached<class Hand>(this);

    // Initialize animation component
    AnimationComp = ETG::CreateGameObjectAttached<BulletManAnimComp>(this);
    AnimationComp->Initialize();
    AnimationComp->Update();

    Gun = ETG::CreateGameObjectAttached<Magnum>(this, Hand->GetRelativePosition());
    Gun->Initialize();

    Gun->ProjTexture->loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Projectiles/Enemy" / "8x8_enemy_projectile_001.png").string());
}

void ETG::BulletMan::Update()
{
    EnemyBase::Update(); // This now includes UpdateForce()
    CollisionComp->Update();

    std::cout << HealthComp->CurrentHealth << std::endl;

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

    // Always update animation and gun regardless of force state
    AnimationComp->Update();
    Gun->Update();

    //Visibility settings
    Gun->IsVisible = !IsDead();
    Hand->IsVisible = !IsDead();
}

void ETG::BulletMan::BulletManShoot()
{
    // Don't shoot if being forced/hit/dead
    if (IsBeingForced() || IsDead()) return;

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
