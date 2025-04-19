#include "AK47.h"
#include <filesystem>
#include "../../Core/Factory.h"
#include "../../Modifiers/Gun/MultiShotModifier.h"
#include "../../Core/Components/CollisionComponent.h"
#include "../../Characters/Hero.h"

ETG::AK47::AK47(const sf::Vector2f& pos) : GunBase(pos,
    0.4f,     // FireRate
    150.0f,     // ShotSpeed
    1000.0f,    // Range (should be infinite but I will just give 2000)
    0.0f,      // timerForVelocity
    3.0f,      // depth
    500,       // MaxAmmo
    30,        // MagazineSize
    2.0f,      // ReloadTime
    2.5f,      // Damage
    17.0f,      // Force
    3.0f)      // Spread (in degrees)
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
    ArrowComp->arrowOriginOffset = {-6.f, 0.f};
    ArrowComp->arrowOffset = {15.f, -2.f};
    CollisionComp->Initialize();

    
    // Load the projectile texture for AK-47
    const auto projPath = (std::filesystem::path(RESOURCE_PATH) / "Projectiles"  / "bullet_variant_002.png").string();
    if (!ProjTexture->loadFromFile(projPath))
        throw std::runtime_error("Failed to load Projectile_AK-47.png from path: " + projPath);

    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        if (auto* hero = dynamic_cast<Hero*>(eventData.Other))
        {
            hero->EquipGun(this);
            CollisionComp->SetCollisionEnabled(false); //After equip
            this->Owner = hero; //Set the owner of the gun to the hero This is important because during projectile collision, we need to know the owner of the projectile
        }
    });

    GunBase::Initialize();
}

void ETG::AK47::Update()
{
    MuzzleFlash->Deactivate();
    MuzzleFlash->IsVisible = false;
    CollisionComp->Update();
    ArrowComp->Update();
    GunBase::Update();
}

void ETG::AK47::Draw()
{
    GunBase::Draw();
    if (CollisionComp) CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
}

ETG::AK47AnimComp::AK47AnimComp()
{
    IsGameObjectUISpecified = true;
    AK47AnimComp::SetAnimations();
}

void ETG::AK47AnimComp::SetAnimations()
{
    BaseAnimComp<GunStateEnum>::SetAnimations();

    // Idle Animation
    const Animation IdleAnim = {Animation::CreateSpriteSheet("Guns/AK47", "AK47_Single001", "png", 0.15f, false)};
    AddGunAnimationForState(GunStateEnum::Idle, IdleAnim);

    // Shoot animations
    const Animation ShootAnim = {Animation::CreateSpriteSheet("Guns/AK47", "ak47_shoot_001", "png", 0.15f)};
    AddGunAnimationForState(GunStateEnum::Shoot, ShootAnim);

    // Reload Animation
    const Animation ReloadAnim = {Animation::CreateSpriteSheet("Guns/AK47", "ak47_reload_001", "png", ReloadAnimInterval, false)};
    AddGunAnimationForState(GunStateEnum::Reload, ReloadAnim);
}