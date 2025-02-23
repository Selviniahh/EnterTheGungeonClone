#include "RogueSpecial.h"

ETG::RogueSpecial::RogueSpecial(const sf::Vector2f& Position) : GunBase(Position, 0.35f, 200.f, 5000.f, 0.f)
{
    AnimationComp = std::make_unique<RogueSpecialAnimComp>();
    AnimationComp->SetAnimations();
    
    // call the common initialization.
    Initialize();
}

void ETG::RogueSpecial::Initialize()
{
    OriginOffset = {1.f, 10.f};
    MuzzleFlashEachFrameSpeed = 0.10f;
    arrowOriginOffset = {-6.f, 0.f};
    arrowOffset = {20.f, -6.f};

    // Set up the muzzle flash animation.
    muzzleFlashAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial/MuzzleFlash", "RS_muzzleflash_001", "png", MuzzleFlashEachFrameSpeed);
    muzzleFlashAnim.Origin = { static_cast<float>(muzzleFlashAnim.Texture.getSize().x / 2),
                               static_cast<float>(muzzleFlashAnim.Texture.getSize().y / 2) };
    muzzleFlashAnim.Active = false;
    MuzzleFlashOffset = {17.f, 0.f};

    // Load the projectile texture for RogueSpecial.
    if (!ProjTexture.loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "RogueSpecial" / "Projectile_RogueSpecial.png").string()))
        throw std::runtime_error("Failed to load Projectile_RogueSpecial.png");

    GunBase::Initialize();
}

ETG::RogueSpecialAnimComp::RogueSpecialAnimComp()
{
    SetAnimations();
}

void ETG::RogueSpecialAnimComp::SetAnimations()
{
    //Idle Animation
    const auto IdleAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial", "RogueSpecial_Idle", "png", 0.15f, true);
    auto IdleAnimManager = AnimationManager();
    IdleAnimManager.AddAnimation(GunStateEnum::Idle, IdleAnim);
    IdleAnimManager.SetOrigin(GunStateEnum::Idle, sf::Vector2f{(float)(IdleAnim.FrameRects[0].width / 2), (float)(IdleAnim.FrameRects[0].width / 2),});

    AnimManagerDict[GunStateEnum::Idle] = IdleAnimManager;

    //Shoot animations
    const auto ShootAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial/Fire", "knav3_fire_001", "png", 0.15f);

    auto ShootAnimManager = AnimationManager();
    ShootAnimManager.AddAnimation(GunStateEnum::Shoot, ShootAnim);
    ShootAnimManager.SetOrigin(GunStateEnum::Shoot, sf::Vector2f{(float)(ShootAnim.FrameRects[0].width / 2), (float)(ShootAnim.FrameRects[0].width / 2),});

    AnimManagerDict[GunStateEnum::Shoot] = ShootAnimManager;

    //Reload Animation
    const auto ReloadAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial", "RogueSpecial_Reload", "png", 0.15f, true);
    auto ReloadAnimManager = AnimationManager();
    ReloadAnimManager.AddAnimation(GunStateEnum::Reload, ReloadAnim);
    ReloadAnimManager.SetOrigin(GunStateEnum::Reload, sf::Vector2f{(float)(ReloadAnim.FrameRects[0].width / 2), (float)(ReloadAnim.FrameRects[0].width / 2),});

    AnimManagerDict[GunStateEnum::Reload] = ReloadAnimManager;
    
}
