#include "GunBase.h"
#include <filesystem>
#include <iostream>
#include <numbers>
#include "../../Managers/Globals.h"
#include "../../Projectile/ProjectileBase.h"

using namespace ETG;

GunBase::GunBase(const sf::Vector2f Position, const float pressTime, const float velocity, const float maxProjectileRange, const float timerForVelocity)
    : pressTime(pressTime), velocity(velocity), maxProjectileRange(maxProjectileRange), timerForVelocity(timerForVelocity)
{
    // Initialize common position.
    this->Position = Position;
}

GunBase::~GunBase()
{
    for (const auto proj : projectiles)
        delete proj;
}

void GunBase::Initialize()
{
    // Set the origin based on the current gun texture.
    Origin = {
        static_cast<float>(AnimationComp->CurrentTex.getSize().x / 2),
        static_cast<float>(AnimationComp->CurrentTex.getSize().y / 2)
    };
    Origin += OriginOffset;

    // Load the arrow texture (common for all guns).
    const std::filesystem::path arrowPath = std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "Arrow.png";
    if (!ArrowTex.loadFromFile(arrowPath.string()))
        throw std::runtime_error(arrowPath.string() + " not found");

    
    arrowOrigin = {
        static_cast<float>(ArrowTex.getSize().x / 2),
        static_cast<float>(ArrowTex.getSize().y / 2)
    };
    arrowOrigin += arrowOriginOffset;

    // Muzzle flash animation should be set up by the derived class.
    muzzleFlashAnim.Active = false;
}

void GunBase::Update()
{
    timerForVelocity += Globals::FrameTick;

    // If the shoot animation finished, revert to idle.
    if (AnimationComp->CurrentGunState == GunStateEnum::Shoot &&
        AnimationComp->AnimManagerDict[AnimationComp->CurrentGunState].IsAnimationFinished())
    {
        CurrentGunState = GunStateEnum::Idle;
    }

    // Update arrow position.
    arrowPos = Position + RotateVector(arrowOffset);

    // Update gun animation.
    AnimationComp->Update(CurrentGunState, CurrentGunState);
    GunTexture = AnimationComp->CurrentTex;

    // Update muzzle flash position and animation.
    MuzzleFlashPos = arrowPos + RotateVector(MuzzleFlashOffset);
    muzzleFlashAnim.Update();
    if (muzzleFlashAnim.Active && muzzleFlashAnim.IsAnimationFinished())
        muzzleFlashAnim.Active = false;

    // Update projectiles.
    for (const auto proj : projectiles)
    {
        proj->Update();
    }
}

void GunBase::Draw()
{
    // Draw the gun.
    AnimationComp->Draw(Position, Origin, Scale, Rotation, 2);
    Globals::DrawSinglePixelAtLoc(Position, {1, 1}, Rotation);

    // Draw the arrow representation.
    Globals::Renderer::SimpleDraw(ArrowTex, arrowPos, Rotation, arrowOrigin);
    Globals::DrawSinglePixelAtLoc(arrowPos, {1, 1}, Rotation);

    // Draw projectiles.
    for (const auto proj : projectiles)
    {
        proj->Draw();
    }

    // Draw the muzzle flash.
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

        // Restart shoot animation.
        AnimationComp->AnimManagerDict[GunStateEnum::Shoot].AnimationDict[GunStateEnum::Shoot].Restart();

        // Calculate projectile velocity.
        const sf::Vector2f spawnPos = arrowPos;
        const float angle = Rotation;
        const float rad = angle * std::numbers::pi / 180.f;
        const sf::Vector2f direction(std::cos(rad), std::sin(rad));
        const sf::Vector2f projVelocity = direction * velocity;

        // Spawn projectile.
        auto* proj = new ProjectileBase(ProjTexture, spawnPos, projVelocity, maxProjectileRange, Rotation);
        projectiles.push_back(proj);

        // Restart muzzle flash animation.
        muzzleFlashAnim.Restart();
    }
}

sf::Vector2f GunBase::RotateVector(const sf::Vector2f& offset) const
{
    const float angleRad = Rotation * (std::numbers::pi / 180.f);
    sf::Vector2f scaledOffset(offset.x * Scale.x, offset.y * Scale.y);

    return {
        scaledOffset.x * std::cos(angleRad) - scaledOffset.y * std::sin(angleRad),
        scaledOffset.x * std::sin(angleRad) + scaledOffset.y * std::cos(angleRad)
    };
}
