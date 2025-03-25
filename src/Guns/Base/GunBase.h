#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GunBase.h"
#include <boost/describe.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "../../Animation/Animation.h"
#include "../../Core/GameObjectBase.h"
#include "../../Core/Components/ArrowComp.h"
#include "../../Managers/StateEnums.h"
#include "../../Core/Components/BaseAnimComp.h"
#include "../../Core/Events/EventDelegate.h"
#include "../VFX/MuzzleFlash.h"
#include "../../Modifiers/ModifierManager.h"
#include "../../Utils/Interface/IGunModifier.h"

namespace ETG
{
    class ReloadSlider;
    class ProjectileBase;
    struct QueuedBullet;
    enum class GunStateEnum;

    class GunBase : public GameObjectBase
    {
    public:
        GunBase(sf::Vector2f Position, float fireRate, float shotSpeed, float range, float timerForVelocity, float depth, int ammoSize, int magazineSize, float reloadTime,
                float damage = 1.0f, float force = 1.0f, float spread = 0.0f);

        ~GunBase() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;
        virtual void PrepareShooting(); //queue the bulletQueue
        virtual void Reload();
        void SetShootSound(const std::string& soundPath);
        void SetReloadSound(const std::string& soundPath);
        void FireBullet(float projectileAngle); //Fire an individual bullet

        //NOTE: only modifier we have so far. I didn't deem necessary to define this as smart pointer 
        ModifierManager<IGunModifier> modifierManager; 
        
        std::vector<QueuedBullet> bulletQueue; //Queue of bullets waiting to be fired
        static constexpr float MULTI_SHOT_DELAY = 0.1f;  // Delay between bullets in seconds

        using GameObjectBase::Rotation; //Make Rotation public in Gunbase

        //State will not contain direction. It will be idle, shoot, reload etc. 
        GunStateEnum CurrentGunState{GunStateEnum::Idle};

        bool IsReloading{};

        //Base stat values (never modified, used as reference)
        float BaseFireRate;
        float BaseShotSpeed;;
        float BaseRange;
        float BaseReloadTime; //Time to reload
        float BaseDamage;
        float BaseForce;
        float BaseSpread;
        
        int BaseMaxAmmo{}; //Total ammo capacity 
        int BaseMagazineSize{}; //Bullets per magazine
        
        //Current effective stats (modified by items/ perks)
        float FireRate; //Time between shots (seconds)
        float ShotSpeed; //How fast bullets travel
        float Range; //How far bullets travel
        float ReloadTime; //Time to reload
        float Damage; //Damage per bullet
        float Force; //Knockback applied to enemies
        float Spread; //Bullet spread angle in degrees (0 = perfect accuracy)

        //Ammo stats
        int MaxAmmo{}; //Total ammo capacity 
        int MagazineSize{}; //Total bullets per magazine
        int MagazineAmmo{}; //Current magazine ammo count (this will be subtracted and reset)

        std::shared_ptr<sf::Texture> ProjTexture;
        std::unique_ptr<ReloadSlider> ReloadSlider;
        EventDelegate<bool> OnAmmoRunOut;
        EventDelegate<bool> OnReloadInvoke;

    protected:
        float Timer; //Based on tick, this will increment 

        // Rotates an offset vector according to the gun's current rotation.
        std::vector<std::unique_ptr<ProjectileBase>> projectiles;
        std::unique_ptr<ArrowComp> ArrowComp;
        std::unique_ptr<MuzzleFlash> MuzzleFlash;

        //Gun needs to have custom Origin offset cuz, it needs to be attached to Hero's hand
        sf::Vector2f OriginOffset;

        //Gun Animation
        std::unique_ptr<BaseAnimComp<GunStateEnum>> AnimationComp;

    private:
        //Sounds
        sf::SoundBuffer ShootSoundBuffer;
        sf::Sound ShootSound;

        sf::SoundBuffer ReloadSoundBuffer;
        sf::Sound ReloadSound;

        float ShootSoundVolume = 30;
        float ReloadSoundVolume = 30;

        BOOST_DESCRIBE_CLASS(GunBase, (GameObjectBase),
                             (CurrentGunState, MaxAmmo, MagazineSize, MagazineAmmo, ReloadTime, IsReloading,
                                 FireRate, ShotSpeed, Range, Damage, Force, Spread),
                             (ProjTexture, OriginOffset),
                             ())
    };

    struct QueuedBullet
    {
        float timeToFire;
        float angle;
    };
}
