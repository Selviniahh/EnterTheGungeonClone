#include <filesystem>
#include "GunBase.h"
#include <random>

#include "../../Characters/Hero.h"
#include "../../Projectile/ProjectileBase.h"
#include "../../Managers/Globals.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Core/Factory.h"
#include "../../UI/UIObjects/ReloadSlider.h"
#include "../../Utils/Math.h"
#include "../../Items/Passive/PlatinumBullets.h"
#include "../../Modifiers/Gun/MultiShotModifier.h"

namespace ETG
{
    GunBase::GunBase(const sf::Vector2f Position,
                     const float fireRate,
                     const float shotSpeed,
                     const float range,
                     const float timerForVelocity,
                     const float depth,
                     const int maxAmmo,
                     const int magazineSize,
                     const float reloadTime,
                     const float damage,
                     const float force,
                     const float spread)
        : BaseFireRate(fireRate), BaseShotSpeed(shotSpeed),
          BaseRange(range), BaseReloadTime(reloadTime), BaseDamage(damage),
          BaseForce(force), BaseSpread(spread),
          BaseMaxAmmo(maxAmmo), BaseMagazineSize(magazineSize), Timer(timerForVelocity)
    {
        // Initialize common position and textures
        this->Position = Position;
        this->Depth = depth;
        this->MagazineAmmo = MagazineSize;

        //Initially base and current states are same
        FireRate = BaseFireRate;
        ShotSpeed = BaseShotSpeed;
        Range = BaseRange;
        ReloadTime = BaseReloadTime;
        Damage = BaseDamage;
        Force = BaseForce;
        Spread = BaseSpread;
        MaxAmmo = BaseMaxAmmo;
        MagazineSize = BaseMagazineSize;
        MagazineAmmo = MagazineSize; //Magazine needs to start with Magazine Ammo

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
        Timer = FireRate + 1; //Set timer to be greater than fire rate so that we can shoot immediately

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

        // Continue with the rest of the update logic
        ArrowComp->SetPosition(this->Position + Math::RotateVector(Rotation, Scale, ArrowComp->arrowOffset));
        ArrowComp->SetRotation(this->GetDrawProperties().Rotation);
        ArrowComp->Update();

        // Update gun animation.
        AnimationComp->Update(CurrentGunState, CurrentGunState);

        // Update muzzle flash position and animation.
        MuzzleFlash->Update();

        // Update projectiles.
        UpdateProjectiles();

        ReloadSlider->Update();
    }

    void GunBase::Draw()
    {
        // Draw projectiles.
        for (const auto& proj : projectiles)
        {
            proj->Draw();
        }

        if (!IsVisible) return; //If dashing, this will be false and self gun shouldn't be drawn however projectiles should be. So we first draw projectiles then we draw self if visible 
        GameObjectBase::Draw();

        // Draw the gun.
        SpriteBatch::Draw(GetDrawProperties());

        // Draw the arrow representation.
        ArrowComp->Draw();

        // Draw the muzzle flash.
        if (MuzzleFlash->IsVisible) MuzzleFlash->Draw();
        ReloadSlider->Draw();
    }

    void GunBase::UpdateProjectiles()
    {
        for (auto it = projectiles.begin(); it != projectiles.end();)
        {
            (*it)->Update();
            if ((*it)->IsPendingDestroy())
            {
                UnregisterGameObject(it->get()->GetObjectName());

                //Because initialized projectile moved to this container with std::move: "projectiles.push_back(std::move(proj));", owner of the object is this container.
                //Simply removing the element from the vector will invoke
                //unique_ptr's destructor because unique_ptr requires 1 owner and since owner is gone, it'll automatically call destructor right away after this erase call.
                it = projectiles.erase(it); //After erase, set iterator to next iterator after the one removed
            }
            else
            {
                ++it;
            }
        }
    }

    void GunBase::PrepareShooting()
    {
        if (Timer >= FireRate)
        {
            //Reset firing timer
            Timer = 0;

            //NOTE: DO NOT MISS HERE!!!!!!! Apply modifiers if present. Later on when all of these modifiers will get complex, a new class that will only handle modifiers should be created. 
            int shotCount = 1;
            float EffectiveSpread = Spread; //Because Spread needs to be reverted back, when modifier overs, in place temp local variable required 
            if (const auto& multiMod = modifierManager.GetModifier<MultiShotModifier>())
            {
                shotCount = multiMod->GetShotCount();
                EffectiveSpread = multiMod->GetSpread();
            }

            //Consume ammo only once per shot group regardless of MultiShotModifier
            MagazineAmmo--;

            EnqueueProjectiles(shotCount, EffectiveSpread);
        }

        //Handle ammo depletion
        if (MagazineAmmo == 0)
        {
            OnAmmoRunOut.Broadcast(true);
        }
    }

    void GunBase::EnqueueProjectiles(const int shotCount, const float EffectiveSpread)
    {
        //Queue any additional bullets with delay (only useful if shotCount > 1) 
        for (int i = 0; i < shotCount; i++)
        {
            float projectileAngle = GameObjectBase::Rotation;

            //Apply spread variation
            if (EffectiveSpread > 0)
            {
                std::mt19937 engine(std::random_device{}());
                std::uniform_real_distribution<float> dist(-EffectiveSpread, EffectiveSpread);
                projectileAngle += dist(engine);
            }

            //Queue the bullet
            bulletQueue.push_back({i * ShotDelay, projectileAngle});
        }
    }

    void GunBase::FireBullet(float projectileAngle)
    {
        //Restart muzzle flash animation and shoot animation
        if (MuzzleFlash->IsVisible) MuzzleFlash->Restart();
        ShootSound.play();
        
        //Set animation state
        CurrentGunState = GunStateEnum::Shoot;
        RestartCurrentAnimStateAnimation();

        //Calculate spawn position
        const sf::Vector2f spawnPos = ArrowComp->GetPosition();

        //Calculate velocity
        const float rad = Math::AngleToRadian(projectileAngle);
        const sf::Vector2f direction = Math::RadianToDirection(rad);
        const sf::Vector2f projVelocity = direction * ShotSpeed;

        //Spawn a projectile NOTE: I decided to instead spawn projectiles attached to fired gun. That's because in collision resolution, I need to know the owner gun of the projectile and from that learn whether it's a hero's or enemy's projectile
        //projectile.
        std::unique_ptr<ProjectileBase> proj = CreateGameObjectAttached<ProjectileBase>(this,*ProjTexture, spawnPos, projVelocity, Range, projectileAngle, Damage, Force);
        proj->Update();
        projectiles.push_back(std::move(proj));
    }

    void GunBase::Reload()
    {
        //IF already reloading or magazine is full do not invoke again
        if (IsReloading || MagazineAmmo == MagazineSize) return;
        
        CurrentGunState = GunStateEnum::Reload; //update animation
        RestartCurrentAnimStateAnimation();
        IsReloading = true;
        ReloadSound.play();
        OnAmmoRunOut.Broadcast(false); // Notify that we have ammo again
        OnReloadInvoke.Broadcast(true);
    }

    void GunBase::RestartCurrentAnimStateAnimation()
    {
        AnimationComp->AnimManagerDict[CurrentGunState].AnimationDict[CurrentGunState].Restart();
    }

    void GunBase::SetShootSound(const std::string& soundPath)
    {
        if (!ShootSoundBuffer.loadFromFile(soundPath))
            throw std::runtime_error("Failed to load gun sound");

        ShootSound.setBuffer(ShootSoundBuffer);
        ShootSound.setVolume(ShootSoundVolume);
    }

    void GunBase::SetReloadSound(const std::string& soundPath)
    {
        if (!ReloadSoundBuffer.loadFromFile(soundPath))
            throw std::runtime_error("Failed to load Reload sound");

        ReloadSound.setBuffer(ReloadSoundBuffer);
        ReloadSound.setVolume(ReloadSoundVolume);
    }
}
