#include "RogueSpecial.h"
#include "../../Core/Factory.h"

#include <filesystem>

ETG::RogueSpecial::RogueSpecial(const sf::Vector2f& Position) : GunBase(Position, 0.35f, 200.f, 5000.f, 0.f)
{
    AnimationComp = CreateGameObjectAttached<RogueSpecialAnimComp>(this);
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
    if (muzzleFlashAnim.Texture) {
        muzzleFlashAnim.Origin = { 
            static_cast<float>(muzzleFlashAnim.Texture->getSize().x / 2),
            static_cast<float>(muzzleFlashAnim.Texture->getSize().y / 2) 
        };
    }
    muzzleFlashAnim.Active = false;
    MuzzleFlashOffset = {17.f, 0.f};

    // Load the projectile texture for RogueSpecial.
    // Fixed: Don't try to load before initializing ProjTexture
    const auto projPath = (std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "RogueSpecial" / "Projectile_RogueSpecial.png").string();
    if (!ProjTexture->loadFromFile(projPath))
        throw std::runtime_error("Failed to load Projectile_RogueSpecial.png from path: " + projPath);

    GunBase::Initialize();
}

ETG::RogueSpecialAnimComp::RogueSpecialAnimComp()
{
    RogueSpecialAnimComp::SetAnimations();
}

void ETG::RogueSpecialAnimComp::SetAnimations()
{
    //Idle Animation
    const auto IdleAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial", "RogueSpecial_Idle", "png", 0.15f, true);
    auto IdleAnimManager = AnimationManager();
    IdleAnimManager.AddAnimation(GunStateEnum::Idle, IdleAnim);
    if (IdleAnim.FrameRects.size() > 0) {
        IdleAnimManager.SetOrigin(GunStateEnum::Idle, sf::Vector2f{
            static_cast<float>(IdleAnim.FrameRects[0].width / 2), 
            static_cast<float>(IdleAnim.FrameRects[0].width / 2)
        });
    }
    AnimManagerDict[GunStateEnum::Idle] = IdleAnimManager;

    //Shoot animations
    const auto ShootAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial/Fire", "knav3_fire_001", "png", 0.15f);
    auto ShootAnimManager = AnimationManager();
    ShootAnimManager.AddAnimation(GunStateEnum::Shoot, ShootAnim);
    if (ShootAnim.FrameRects.size() > 0) {
        ShootAnimManager.SetOrigin(GunStateEnum::Shoot, sf::Vector2f{
            static_cast<float>(ShootAnim.FrameRects[0].width / 2), 
            static_cast<float>(ShootAnim.FrameRects[0].width / 2)
        });
    }
    AnimManagerDict[GunStateEnum::Shoot] = ShootAnimManager;

    //Reload Animation
    const auto ReloadAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial", "RogueSpecial_Reload", "png", 0.15f, true);
    auto ReloadAnimManager = AnimationManager();
    ReloadAnimManager.AddAnimation(GunStateEnum::Reload, ReloadAnim);
    if (ReloadAnim.FrameRects.size() > 0) {
        ReloadAnimManager.SetOrigin(GunStateEnum::Reload, sf::Vector2f{
            static_cast<float>(ReloadAnim.FrameRects[0].width / 2), 
            static_cast<float>(ReloadAnim.FrameRects[0].width / 2)
        });
    }
    AnimManagerDict[GunStateEnum::Reload] = ReloadAnimManager;
}