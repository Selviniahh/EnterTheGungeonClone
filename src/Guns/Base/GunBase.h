#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GunBase.h"
#include <boost/describe.hpp>
#include "../../Animation/Animation.h"
#include "../../Core/GameObjectBase.h"
#include "../../Core/Components/ArrowComp.h"
#include "../../Managers/StateEnums.h"
#include "../../Core/Components/BaseAnimComp.h"
#include "../../Core/Events/EventDelegate.h"
#include "../VFX/MuzzleFlash.h"

class ProjectileBase;

namespace ETG
{
    enum class GunStateEnum;
    
    class GunBase : public GameObjectBase
    {
    public:
        GunBase(sf::Vector2f Position, float pressTime, float velocity, float maxProjectileRange, float timerForVelocity, int Depth, int ammoSize, int magazineSize);
        ~GunBase() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;
        virtual void Shoot();
        virtual void Reload();

        using GameObjectBase::Rotation; //Make Rotation public in Gunbase

        //Total ammo count 
        int AmmoSize{};

        //Total magazine count
        int MagazineSize{};

        //Current magazine ammo count (this will be subtracted and reset)
        int MagazineAmmo{};

        std::shared_ptr<sf::Texture> ProjTexture;
        EventDelegate<bool> OnAmmoStateChanged;

    protected:
        // Rotates an offset vector according to the gun's current rotation.
        [[nodiscard]] sf::Vector2f RotateVector(const sf::Vector2f& offset) const;
        std::vector<std::unique_ptr<ProjectileBase>> projectiles;
        std::unique_ptr<ArrowComp> ArrowComp;
        std::unique_ptr<MuzzleFlash> MuzzleFlash;

        // Muzzle flash variables (instance sets up its own animation).
        float pressTime;
        float velocity;
        float maxProjectileRange;
        float timerForVelocity;
        bool isAttacking{};

        //Gun needs to have custom Origin offset cuz, it needs to be attached to Hero's hand
        sf::Vector2f OriginOffset;
        
        //Gun Animation
        std::unique_ptr<BaseAnimComp<GunStateEnum>> AnimationComp;
        GunStateEnum CurrentGunState{GunStateEnum::Idle};

        BOOST_DESCRIBE_CLASS(GunBase, (GameObjectBase), (),
            (ProjTexture, pressTime, velocity, maxProjectileRange, timerForVelocity, isAttacking, OriginOffset),
            ())         //TODO Later expose ArrowComp and muzzleflash to the Gun 
    };
}