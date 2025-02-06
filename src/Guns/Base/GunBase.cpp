#include "GunBase.h"
#include <filesystem>
#include <iostream>
#include <numbers>
#include "../../Managers/Globals.h"
#include "../../Projectile/ProjectileBase.h"

using namespace ETG;

GunBase::GunBase(const sf::Vector2f Position) : pressTime(2.f), velocity(200.f), maxProjectileRange(5000.f), timerForVelocity(0.f), isAttacking(false)
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
    Origin += sf::Vector2f{1, 10};

    //Set Arrow properties
    const std::filesystem::path FullPath = std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "Arrow.png";
    if (!ArrowTex.loadFromFile(FullPath.string()))
        throw std::runtime_error(FullPath.string() + " Not found");

    arrowOffset = sf::Vector2f(20.f, -5.f);
    arrowOrigin = {static_cast<float>(ArrowTex.getSize().x / 2), static_cast<float>(ArrowTex.getSize().y / 2)};
    arrowOrigin.x += -6;
}

void GunBase::Update()
{
    // muzzleFlashAnim.Update();

    timerForVelocity += Globals::FrameTick;
    // Rotation += Globals::FrameTick * 35;

    if (timerForVelocity >= pressTime)
    {
        //Set animation to Shoot
        CurrentGunState = GunStateEnum::Shoot;

        timerForVelocity = 0;
        Shoot();
    }

    //If animation is Shoot and if animation is finished, then set it back to idle
    if (AnimationComp.CurrentGunState == GunStateEnum::Shoot && AnimationComp.AnimManagerDict[AnimationComp.CurrentGunState].IsAnimationFinished())
    {
        CurrentGunState = GunStateEnum::Idle;
    }

    //Set arrow position
    arrowPos = Position + RotateVector(arrowOffset);

    //Set animations
    AnimationKey animState = GunStateEnum::Idle;

    if (CurrentGunState == GunStateEnum::Idle) animState = GunStateEnum::Idle;
    if (CurrentGunState == GunStateEnum::Shoot) animState = GunStateEnum::Shoot;
    if (CurrentGunState == GunStateEnum::Reload) animState = GunStateEnum::Reload;

    AnimationComp.Update(CurrentGunState, animState);
    GunTexture = AnimationComp.CurrentTex;

    //Update proectiles
    for (const auto proj : projectiles)
    {
        proj->Update();
    }
}

void GunBase::Draw()
{
    //Draw self gun
    AnimationComp.Draw(Position, Origin, Scale, Rotation,2);
    Globals::DrawSinglePixelAtLoc(Position, {1, 1}, Rotation);

    //Draw Arrow Representative
    Globals::Renderer::SimpleDraw(ArrowTex, arrowPos, Rotation, arrowOrigin);
    Globals::DrawSinglePixelAtLoc(arrowPos, {1, 1}, Rotation);

    //Draw projectiles
    for (const auto proj : projectiles)
    {
        proj->Draw();
    }
}

void GunBase::Shoot()
{
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

sf::Vector2f GunBase::RotateVector(const sf::Vector2f& arrowOffset) const
{
    const float angleRad = Rotation * (std::numbers::pi / 180.f);
    sf::Vector2f scaledOffset(arrowOffset.x * Scale.x, arrowOffset.y * Scale.y);
    
    return sf::Vector2f(
        scaledOffset.x * std::cos(angleRad) - scaledOffset.y * std::sin(angleRad),
        scaledOffset.x * std::sin(angleRad) + scaledOffset.y * std::cos(angleRad)
    );
}
