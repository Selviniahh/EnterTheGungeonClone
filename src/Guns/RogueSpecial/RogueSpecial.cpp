#include "RogueSpecial.h"
#include <filesystem>
#include "../../Core/Factory.h"
#include "../../Modifiers/Gun/MultiShotModifier.h"

ETG::RogueSpecial::RogueSpecial(const sf::Vector2f& Position) : GunBase(Position,
0.35f, 200.f, 300.f, 0.f,2,300,10, 3.0f, 3.5f, 2.f, 10.f)
{
    AnimationComp = CreateGameObjectAttached<RogueSpecialAnimComp>(this);
    SetShootSound((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "RogueSpecialShoot.ogg").generic_string());
    SetReloadSound((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "Reload.ogg").generic_string());

    // call the common initialization.
    RogueSpecial::Initialize();
}

void ETG::RogueSpecial::Initialize()
{
    OriginOffset = {1.f, 10.f};
    ArrowComp->arrowOriginOffset = {-6.f, 0.f};
    ArrowComp->arrowOffset = {20.f, -6.f};

    // Set up the muzzle flash animation.
    MuzzleFlash->Deactivate();
    MuzzleFlash->SetOffset({37.f, -6.f});

    // Load the projectile texture for RogueSpecial.
    const auto projPath = (std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "RogueSpecial" / "Projectile_RogueSpecial.png").string();
    if (!ProjTexture->loadFromFile(projPath))
        throw std::runtime_error("Failed to load Projectile_RogueSpecial.png from path: " + projPath);

    GunBase::Initialize();
}

void ETG::RogueSpecial::Update()
{
    GunBase::Update();

    if (modifierManager.GetModifier<MultiShotModifier>())
    {
        // When multishot is active, make flash animation match bullet frequency
        MuzzleFlash->Animation.FrameInterval = MULTI_SHOT_DELAY / 2;
    }
    else
    {
        // Normal animation speed for single shots
        MuzzleFlash->Animation.FrameInterval = FireRate / 3;
    }
}

ETG::RogueSpecialAnimComp::RogueSpecialAnimComp()
{
    IsGameObjectUISpecified = true;
    RogueSpecialAnimComp::SetAnimations();
}

void ETG::RogueSpecialAnimComp::SetAnimations()
{
    //Idle Animation
    const Animation IdleAnim = {Animation::CreateSpriteSheet("Guns/RogueSpecial", "RogueSpecial_Idle", "png", 0.15f, true)};
    AddGunAnimationForState(GunStateEnum::Idle, IdleAnim);

    //Shoot animations
    const Animation ShootAnim = {Animation::CreateSpriteSheet("Guns/RogueSpecial/Fire", "knav3_fire_001", "png", 0.15f)};
    AddGunAnimationForState(GunStateEnum::Shoot, ShootAnim);

    //Reload Animation
    const Animation ReloadAnim = {Animation::CreateSpriteSheet("Guns/RogueSpecial", "RogueSpecial_Reload", "png", 0.15f, true)};
    AddGunAnimationForState(GunStateEnum::Reload, ReloadAnim);
}
