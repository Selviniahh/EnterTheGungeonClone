#pragma once
#include "GunAnimComp.h"
#include "../../Animation/Animation.h"
#include "../../Core/GameObject.h"

class ProjectileBase;

namespace ETG
{
    class GunBase : public GameObject
    {
    public:
        explicit GunBase(sf::Vector2f Position, float pressTime, float velocity, float maxProjectileRange, float timerForVelocity, bool isAttacking);
        ~GunBase() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;
        void Shoot();
        
        // void Shoot(const sf::Vector2f& projectileVelocity);
        // void Shoot(const sf::Vector2f& projectileVelocity, const sf::Vector2f& spawnLocation);
        // void Shoot(const sf::Vector2f& projectileVelocity, const sf::Vector2f& spawnLocation, float delay, size_t numberOfProjectiles);

    public:
        float Rotation{};
        
    protected:
        sf::Vector2f RotateVector(const sf::Vector2f& arrowOffset) const;
        
        std::vector<ProjectileBase*> projectiles;

        sf::Texture GunTexture;
        sf::Texture ProjTexture;

        //Muzzle Flash
        Animation muzzleFlashAnim;
        float MuzzleFlashEachFrameSpeed = 0.10f;
        sf::Vector2f MuzzleFlashPos;
        sf::Vector2f MuzzleFlashOffset;

        float pressTime;
        float velocity;
        float maxProjectileRange;
        float timerForVelocity;
        float isAttacking;

        sf::Vector2f OriginOffset{1.f, 10.f};

        //Arrow
        sf::Texture ArrowTex;
        sf::Vector2f arrowOffset;
        sf::Vector2f arrowOrigin;
        sf::Vector2f arrowOriginOffset{-6,0};
        sf::Vector2f arrowPos;

        GunAnimComp AnimationComp;
        GunStateEnum CurrentGunState{GunStateEnum::Idle};
    };
}
