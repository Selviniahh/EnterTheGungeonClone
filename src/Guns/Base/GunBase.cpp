#include <complex>
#include <filesystem>
#include "GunBase.h"
#include <numbers>
#include <random>

#include "../../Projectile/ProjectileBase.h"
#include "../../Managers/Globals.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Core/Factory.h"
#include "../../UI/UIObjects/ReloadSlider.h"
#include "../../Utils/Math.h"

namespace ETG
{
    GunBase::GunBase(const sf::Vector2f Position,
                     const float fireRate,
                     const float shotSpeed,
                     const float range,
                     const float timerForVelocity,
                     float depth,
                     const int maxAmmo,
                     const int magazineSize,
                     const float reloadTime,
                     const float damage,
                     const float force,
                     const float spread)
        : MaxAmmo(maxAmmo), MagazineSize(magazineSize),
          FireRate(fireRate), ShotSpeed(shotSpeed), Range(range),
          Timer(timerForVelocity), ReloadTime(reloadTime),
          Damage(damage), Force(force), Spread(spread)
    {
        // Initialize common position and textures
        this->Position = Position;
        this->Depth = depth;
        this->MagazineAmmo = MagazineSize;

        if (!Texture) Texture = std::make_shared<sf::Texture>();
        if (!ProjTexture) ProjTexture = std::make_shared<sf::Texture>();
        if (!Texture) Texture = std::make_shared<sf::Texture>();
        if (!ArrowComp) ArrowComp = CreateGameObjectAttached<class ArrowComp>(this, (std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "Arrow.png").string());
        if (!MuzzleFlash) MuzzleFlash = CreateGameObjectAttached<class MuzzleFlash>(this, "Guns/RogueSpecial/MuzzleFlash/", "RS_muzzleflash_001", "png", 0.10f);
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

        Timer += Globals::FrameTick;

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
        if (Timer >= FireRate)
        {
            MagazineAmmo--;

            // Restart muzzle flash animation.
            MuzzleFlash->Restart();

            //Set animation to Shoot
            CurrentGunState = GunStateEnum::Shoot;
            Timer = 0;

            //Calculate projectile velocity with spread
            const sf::Vector2f spawnPos = ArrowComp->GetPosition();
            float ProjectileAngle = Rotation;

            //Apply some kind of recoil with randomly incrementing Spread to angle
            if (Spread > 0)
            {
                std::mt19937 engine(std::random_device{}());
                std::uniform_real_distribution<float> dist(-Spread, Spread);
                ProjectileAngle += dist(engine);
            }

            // Restart shoot animation. //TODO: How can I fix this weird double map? Hero, enemies all needs double dictionary. In the future, more complex guns will also require to have complex animations as well. 
            AnimationComp->AnimManagerDict[GunStateEnum::Shoot].AnimationDict[GunStateEnum::Shoot].Restart();

            // Calculate projectile velocity.
            const float rad = Math::AngleToRadian(ProjectileAngle);
            const sf::Vector2f direction = Math::RadianToDirection(rad);
            const sf::Vector2f projVelocity = direction * ShotSpeed;

            // Spawn projectile.
            std::unique_ptr<ProjectileBase> proj = ETG::CreateGameObjectDefault<ProjectileBase>(*ProjTexture, spawnPos, projVelocity, Range, ProjectileAngle, Damage, Force);
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
