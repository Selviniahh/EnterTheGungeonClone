#include "GunBase.h"
#include <filesystem>
#include <iostream>
#include <numbers>
#include "../../Managers/Globals.h"
#include "../../Projectile/ProjectileBase.h"

using namespace ETG;

GunBase::GunBase(const sf::Vector2f Position, const float pressTime, const float velocity, const float maxProjectileRange, const float timerForVelocity, const bool isAttacking) : pressTime(pressTime), velocity(velocity),
    maxProjectileRange(maxProjectileRange),
    timerForVelocity(timerForVelocity), isAttacking(isAttacking)
{
    //Init Position
    this->Position = Position;

    //Init Gun Animations
    AnimationComp.SetAnimations();
    GunBase::Initialize();

    //Init Projectile texture
    ProjTexture.loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "Projectile_RogueSpecial.PNG").string());
}

GunBase::~GunBase()
{
    for (const auto proj : projectiles)
        delete proj;
}

void GunBase::Initialize()
{
    //Muzzle flash animations

    //This is where projectiles will spawn
    Origin = {static_cast<float>(AnimationComp.CurrentTex.getSize().x / 2), static_cast<float>(AnimationComp.CurrentTex.getSize().y / 2)};
    Origin += OriginOffset;

    //Set Arrow properties
    const std::filesystem::path ArrowFullPath = std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "Arrow.png";
    if (!ArrowTex.loadFromFile(ArrowFullPath.string()))
        throw std::runtime_error(ArrowFullPath.string() + " Not found");

    arrowOffset = sf::Vector2f(20.f, -6.f);
    arrowOrigin = {static_cast<float>(ArrowTex.getSize().x / 2), static_cast<float>(ArrowTex.getSize().y / 2)};
    arrowOrigin += arrowOriginOffset;

    //Set Muzzle Flash properties
    muzzleFlashAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial/MuzzleFlash", "RS_muzzleflash_001", "png", MuzzleFlashEachFrameSpeed);
    muzzleFlashAnim.Origin = {static_cast<float>(muzzleFlashAnim.Texture.getSize().x / 2), static_cast<float>(muzzleFlashAnim.Texture.getSize().y / 2)};
    muzzleFlashAnim.Active = false;
    MuzzleFlashOffset = sf::Vector2f{17, 0};
}

void GunBase::Update()
{
    // muzzleFlashAnim.Update();

    timerForVelocity += Globals::FrameTick;


    //If the shoot animation has finished, and GunState is Shoot, revert to idle
    if (AnimationComp.CurrentGunState == GunStateEnum::Shoot && AnimationComp.AnimManagerDict[AnimationComp.CurrentGunState].IsAnimationFinished())
    {
        CurrentGunState = GunStateEnum::Idle;
    }

    //Set arrow position
    arrowPos = Position + RotateVector(arrowOffset);

    //Update Gun Animation
    AnimationComp.Update(CurrentGunState, CurrentGunState);
    GunTexture = AnimationComp.CurrentTex;

    //Update Muzzle Flash Pos
    MuzzleFlashPos = arrowPos + RotateVector(MuzzleFlashOffset);
    muzzleFlashAnim.Update();

    //Play once the muzzle so after the animation finished, set it back to Active. 
    if (muzzleFlashAnim.Active && muzzleFlashAnim.IsAnimationFinished())
        muzzleFlashAnim.Active = false;

    //Update proectiles
    for (const auto proj : projectiles)
    {
        proj->Update();
    }
}

void GunBase::Draw()
{
    //Draw self gun
    AnimationComp.Draw(Position, Origin, Scale, Rotation, 2);
    Globals::DrawSinglePixelAtLoc(Position, {1, 1}, Rotation);

    //Draw Arrow Representative
    Globals::Renderer::SimpleDraw(ArrowTex, arrowPos, Rotation, arrowOrigin);
    Globals::DrawSinglePixelAtLoc(arrowPos, {1, 1}, Rotation);

    //Draw projectiles
    for (const auto proj : projectiles)
    {
        proj->Draw();
    }

    //Draw muzzle flash
    muzzleFlashAnim.Draw(MuzzleFlashPos, 3, Rotation);
}

void GunBase::Shoot()
{
    if (timerForVelocity >= pressTime)
    {
        //Set muzzleFlashAnim Active to true. Once the animation is drawn, it will be set back to false. 
        muzzleFlashAnim.Active = true;
        
        //Set animation to Shoot
        CurrentGunState = GunStateEnum::Shoot;
        timerForVelocity = 0;

        // Force the shoot animation to restart each time being shot
        AnimationComp.AnimManagerDict[GunStateEnum::Shoot].AnimationDict[GunStateEnum::Shoot].Restart();

        // Calculate projectile velocity from the gun’s rotation.
        const sf::Vector2f spawnPos = arrowPos;
        const float angle = Rotation;
        const float rad = angle * std::numbers::pi / 180.f;
        const sf::Vector2f direction(std::cos(rad), std::sin(rad));
        const sf::Vector2f projVelocity = direction * velocity;

        //Spawn projectile
        ProjectileBase* proj = new ProjectileBase(ProjTexture, spawnPos, projVelocity, maxProjectileRange, Rotation);
        projectiles.push_back(proj);

        //Restart muzzle VFX
        muzzleFlashAnim.Restart();
    }
}

sf::Vector2f GunBase::RotateVector(const sf::Vector2f& arrowOffset) const
{
    const float angleRad = Rotation * (std::numbers::pi / 180.f);
    sf::Vector2f scaledOffset(arrowOffset.x * Scale.x, arrowOffset.y * Scale.y);

    return sf::Vector2f(
        scaledOffset.x * std::cos(angleRad) - scaledOffset.y * std::sin(angleRad),
        scaledOffset.x * std::sin(angleRad) + scaledOffset.y * std::cos(angleRad)
    );
}
