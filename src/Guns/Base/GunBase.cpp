#include <complex>
#include <filesystem>
#include "GunBase.h"
#include <numbers>
#include "../../Projectile/ProjectileBase.h"
#include "../../Managers/Globals.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Core/Factory.h"

namespace ETG
{
    GunBase::GunBase(const sf::Vector2f Position, const float pressTime, const float velocity, const float maxProjectileRange, const float timerForVelocity, int Depth, const int ammoSize, const int magazineSize)
        : AmmoSize(ammoSize), MagazineSize(magazineSize), pressTime(pressTime), velocity(velocity), maxProjectileRange(maxProjectileRange), timerForVelocity(timerForVelocity)
    {
        // Initialize common position and textures
        this->Position = Position;
        this->Depth = Depth;
        this->MagazineAmmo = MagazineSize;

        if (!Texture) Texture = std::make_shared<sf::Texture>();
        if (!ProjTexture) ProjTexture = std::make_shared<sf::Texture>();
        if (!Texture) Texture = std::make_shared<sf::Texture>();
        if (!ArrowComp) ArrowComp = CreateGameObjectAttached<class ArrowComp>(this, std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "Arrow.png");
        if (!MuzzleFlash) MuzzleFlash = CreateGameObjectAttached<class MuzzleFlash>(this, "Guns/RogueSpecial/MuzzleFlash/", "RS_muzzleflash_001", "png");

        GunBase::Initialize();
    }

    GunBase::~GunBase()
    {
        for (auto& proj : projectiles)
            proj.reset();
    }

    void GunBase::Initialize()
    {
        //The origin manually needs to be given because when gun rotating, it has to rotate around the attachment point which is the handle point of the gun. 
        this->Origin += OriginOffset;
        ArrowComp->SetOrigin(ArrowComp->GetOrigin() + ArrowComp->arrowOriginOffset);

        // Muzzle flash animation should be set up by the derived class.
        MuzzleFlash->SetParent(this);
    }

    void GunBase::Update()
    {
        GameObjectBase::Update();

        timerForVelocity += Globals::FrameTick;

        // If the shoot animation finished, revert to idle.
        if (AnimationComp->CurrentState == GunStateEnum::Shoot && AnimationComp->AnimManagerDict[AnimationComp->CurrentState].IsAnimationFinished())
        {
            CurrentGunState = GunStateEnum::Idle;
        }

        // Update arrow properties.
        ArrowComp->SetPosition(this->Position + RotateVector(ArrowComp->arrowOffset));
        ArrowComp->SetRotation(this->GetDrawProperties().Rotation);
        ArrowComp->Update();

        // Update gun animation.
        AnimationComp->Update(CurrentGunState, CurrentGunState);

        // Update muzzle flash position and animation.
        MuzzleFlash->Update();

        // Update projectiles.
        for (const auto& proj : projectiles)
        {
            proj->Update();
        }

        //If R pressed, reload
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            Reload();
        }
    }

    void GunBase::Shoot()
    {
        if (timerForVelocity >= pressTime)
        {
            MagazineAmmo--;

            // Restart muzzle flash animation.
            MuzzleFlash->Restart();

            //Set animation to Shoot
            CurrentGunState = GunStateEnum::Shoot;
            timerForVelocity = 0;

            // Restart shoot animation.
            AnimationComp->AnimManagerDict[GunStateEnum::Shoot].AnimationDict[GunStateEnum::Shoot].Restart();

            // Calculate projectile velocity.
            const sf::Vector2f spawnPos = ArrowComp->GetPosition();
            const float angle = Rotation;
            const float rad = angle * std::numbers::pi / 180.f;
            const sf::Vector2f direction(std::cos(rad), std::sin(rad));
            const sf::Vector2f projVelocity = direction * velocity;

            // Spawn projectile.
            std::unique_ptr<ProjectileBase> proj = ETG::CreateGameObjectDefault<ProjectileBase>(*ProjTexture, spawnPos, projVelocity, maxProjectileRange, Rotation);
            proj->Update(); //Necessary to set initial position
            projectiles.push_back(std::move(proj));
        }

        if (MagazineAmmo == 0)
        {
            OnAmmoStateChanged.Broadcast(true);
        }
    }

    void GunBase::Reload()
    {
        //Some reload logic
        AmmoSize -= MagazineSize - MagazineAmmo; //Set ammoSize
        MagazineAmmo = MagazineSize;
        OnAmmoStateChanged.Broadcast(false); // Notify that we have ammo again
    }

    void GunBase::Draw()
    {
        GameObjectBase::Draw();

        // Draw the gun.
        SpriteBatch::Draw(GetDrawProperties());

        // Draw the arrow representation.
        ArrowComp->Draw();

        // Draw projectiles.
        for (const auto& proj : projectiles)
        {
            proj->Draw();
        }

        // Draw the muzzle flash.
        MuzzleFlash->Draw();
    }

    sf::Vector2f GunBase::RotateVector(const sf::Vector2f& offset) const
    {
        const float angleRad = Rotation * (std::numbers::pi / 180.f);
        sf::Vector2f scaledOffset(offset.x * this->Scale.x, offset.y * this->Scale.y);

        return {
            scaledOffset.x * std::cos(angleRad) - scaledOffset.y * std::sin(angleRad),
            scaledOffset.x * std::sin(angleRad) + scaledOffset.y * std::cos(angleRad)
        };
    }
}
