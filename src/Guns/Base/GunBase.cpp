#include <complex>
#include <filesystem>
#include "GunBase.h"
#include <numbers>
#include "../../Projectile/ProjectileBase.h"
#include "../../Managers/Globals.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Core/Factory.h"
#include "../../UI/UIObjects/ReloadSlider.h"
#include "../../Utils/Math.h"

namespace ETG
{
    GunBase::GunBase(const sf::Vector2f Position, const float pressTime, const float velocity, const float maxProjectileRange, const float timerForVelocity, int Depth, const int ammoSize, const int magazineSize, const float reloadTime)
        : AmmoSize(ammoSize), MagazineSize(magazineSize), pressTime(pressTime), velocity(velocity), maxProjectileRange(maxProjectileRange), timerForVelocity(timerForVelocity), ReloadTime(reloadTime)
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
        ReloadSlider = ETG::CreateGameObjectAttached<class ReloadSlider>(this);

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

        ReloadSlider->LinkToGun(this);
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
        ArrowComp->SetPosition(this->Position + Math::RotateVector(Rotation, Scale, ArrowComp->arrowOffset));
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
            CurrentGunState = GunStateEnum::Reload; //update animation
            Reload();
        }
        ReloadSlider->Update();
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

            // Restart shoot animation. //TODO: How can I fix this weird double map? Hero, enemies all needs double dictionary. In the future, more complex guns will also require to have complex animations as well. 
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
            OnAmmoRunOut.Broadcast(true);
        }
    }

    void GunBase::Reload()
    {
        IsReloading = true;
        OnAmmoRunOut.Broadcast(false); // Notify that we have ammo again
        OnReloadInvoke.Broadcast(true);
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
        ReloadSlider->Draw();
    }
}
