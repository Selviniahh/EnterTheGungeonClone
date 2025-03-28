#include "AK47.h"
#include <filesystem>
#include "../../Core/Factory.h"
#include "../../Modifiers/Gun/MultiShotModifier.h"
#include "../../Core/Components/CollisionComponent.h"
#include "../../Characters/Hero.h"

ETG::AK47::AK47(const sf::Vector2f& pos) : GunBase(pos,
    0.11f,     // FireRate
    23.0f,     // ShotSpeed
    2000.0f,    // Range (should be infinite but I will just give 2000)
    0.0f,      // timerForVelocity
    2.0f,      // depth
    500,       // MaxAmmo
    30,        // MagazineSize
    0.5f,      // ReloadTime
    5.5f,      // Damage
    9.0f,      // Force
    4.0f)      // Spread (in degrees)
{
    AnimationComp = CreateGameObjectAttached<AK47AnimComp>(this);
    SetShootSound((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "AK47Shoot.ogg").generic_string());
    SetReloadSound((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "AK47Reload.ogg").generic_string());
    
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 1.f;
    CollisionComp->SetCollisionEnabled(true);

    // Call the common initialization
    AK47::Initialize();
}

void ETG::AK47::Initialize()
{
    OriginOffset = {1.f, 10.f};
    ArrowComp->arrowOriginOffset = {-6.f, 0.f};
    ArrowComp->arrowOffset = {25.f, -6.f};

    // Load the projectile texture for AK-47
    const auto projPath = (std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "AK-47" / "Projectile_AK-47.png").string();
    if (!ProjTexture->loadFromFile(projPath))
        throw std::runtime_error("Failed to load Projectile_AK-47.png from path: " + projPath);

    // --- NEW: Set up collision event to equip the gun on overlap ---
    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        if (auto* hero = dynamic_cast<Hero*>(eventData.Other))
        {
            hero->EquipGun(this);
            CollisionComp->SetCollisionEnabled(false); //After equip 
        }
    });

    GunBase::Initialize();
}

void ETG::AK47::Update()
{
    GunBase::Update();

    
}

ETG::AK47AnimComp::AK47AnimComp()
{
    IsGameObjectUISpecified = true;
    AK47AnimComp::SetAnimations();
}

void ETG::AK47AnimComp::SetAnimations()
{
    // Idle Animation
    const Animation IdleAnim = {Animation::CreateSpriteSheet("Guns/AK47", "AK47_001", "png", 0.15f, true)};
    AddGunAnimationForState(GunStateEnum::Idle, IdleAnim);

    // Shoot animations
    const Animation ShootAnim = {Animation::CreateSpriteSheet("Guns/AK47", "ak47_shoot_001", "png", 0.08f)};
    AddGunAnimationForState(GunStateEnum::Shoot, ShootAnim);

    // Reload Animation
    const Animation ReloadAnim = {Animation::CreateSpriteSheet("Guns/AK47", "ak47_reload_001", "png", 0.10f, true)};
    AddGunAnimationForState(GunStateEnum::Reload, ReloadAnim);
}