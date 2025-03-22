#include <filesystem>
#include "GunBase.h"
#include <random>
#include "../../Projectile/ProjectileBase.h"
#include "../../Managers/Globals.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Core/Factory.h"
#include "../../UI/UIObjects/ReloadSlider.h"
#include "../../Utils/Math.h"
#include "../../Items/Active/DoubleShoot.h"

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
        : FireRate(fireRate), ShotSpeed(shotSpeed),
          Range(range), ReloadTime(reloadTime), Damage(damage),
          Force(force), Spread(spread),
          MaxAmmo(maxAmmo), MagazineSize(magazineSize), Timer(timerForVelocity)
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

        //Fire all the bullets inside bulletQueue and remove them from the vector
        if (!bulletQueue.empty())
        {
            for (auto it = bulletQueue.begin(); it != bulletQueue.end();)
            {
                it->timeToFire -= Globals::FrameTick;
                if (it->timeToFire <= 0)
                {
                    //Time to fie this bullet
                    FireBullet(it->angle);

                    it = bulletQueue.erase(it);
                }
                else
                    ++it;
            }
        }

        // After shooting, reset back to idle
        if (AnimationComp->CurrentState == GunStateEnum::Shoot &&
            AnimationComp->AnimManagerDict[AnimationComp->CurrentState].IsAnimationFinished())
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

    void GunBase::Shoot()
    {
        if (Timer >= FireRate)
        {
            //Reset firing timer
            Timer = 0;

            //NOTE: Apply modifiers if present. Later on when all of these modifiers will get complex. A new class that will only handle modifiers should be created. 
            int shotCount = 1;
            if (const auto multiMod = GetModifier<MultiShotModifier>())
            {
                shotCount = multiMod->GetShotCount();
            }

            //Consume ammo only once per shot group
            MagazineAmmo--;

            //Queue any additional bullets with delay
            for (int i = 0; i < shotCount; i++)
            {
                float projectileAngle = Rotation;

                //Apply spread variation
                if (Spread > 0)
                {
                    std::mt19937 engine(std::random_device{}());
                    std::uniform_real_distribution<float> dist(-Spread, Spread);
                    projectileAngle += dist(engine);
                }

                //Queue the bullet
                bulletQueue.push_back({i * MULTI_SHOT_DELAY, projectileAngle});
            }
        }

        //Handle ammo depletion
        if (MagazineAmmo == 0)
        {
            OnAmmoRunOut.Broadcast(true);
        }
    }

    void GunBase::FireBullet(float projectileAngle)
    {
        //Restart muzzle flash animation and shoot animation
        MuzzleFlash->Restart();
        AnimationComp->AnimManagerDict[GunStateEnum::Shoot].AnimationDict[GunStateEnum::Shoot].Restart();

        //Set animation state
        CurrentGunState = GunStateEnum::Shoot;

        //Calculate spawn position
        const sf::Vector2f spawnPos = ArrowComp->GetPosition();

        //Calculate velocity
        const float rad = Math::AngleToRadian(projectileAngle);
        const sf::Vector2f direction = Math::RadianToDirection(rad);
        const sf::Vector2f projVelocity = direction * ShotSpeed;

        //Spawn a projectile
        std::unique_ptr<ProjectileBase> proj = CreateGameObjectDefault<ProjectileBase>(*ProjTexture, spawnPos, projVelocity, Range, projectileAngle, Damage, Force);
        proj->Update();
        projectiles.push_back(std::move(proj));
    }

    void GunBase::Reload()
    {
        IsReloading = true;
        OnAmmoRunOut.Broadcast(false); // Notify that we have ammo again
        OnReloadInvoke.Broadcast(true);
    }

    void GunBase::AddModifier(const std::shared_ptr<GunModifier>& modifier)
    {
        modifierManager.AddModifier(modifier);
    }

    void GunBase::RemoveModifier(const std::string& modifierName)
    {
        modifierManager.RemoveModifier(modifierName);
    }

    bool GunBase::HasModifier(const std::string& modifierName) const
    {
        return modifierManager.HasModifier(modifierName);
    }
}
