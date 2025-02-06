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
        GunBase(sf::Vector2f Position);
        ~GunBase() override;
        void Draw() override;
        void Update() override;
        void Initialize() override;

        void Shoot();
        // void Shoot(const sf::Vector2f& projectileVelocity);
        // void Shoot(const sf::Vector2f& projectileVelocity, const sf::Vector2f& spawnLocation);
        // void Shoot(const sf::Vector2f& projectileVelocity, const sf::Vector2f& spawnLocation, float delay, size_t numberOfProjectiles);

    public:
        float Rotation{};
        
    protected:
        std::vector<ProjectileBase*> projectiles;

        sf::Texture GunTexture;
        sf::Texture ProjTexture;
        Animation muzzleFlashAnim;

        float pressTime;
        float velocity;
        float maxProjectileRange;
        float timerForVelocity;
        float isAttacking;

        //Arrow
        sf::Texture ArrowTex;
        sf::Vector2f arrowOffset;
        sf::Vector2f arrowOrigin;
        sf::Vector2f arrowPos;

        sf::Vector2f RotateVector(const sf::Vector2f& arrowOffset) const;

        // later on draw line trace starting from Gun position and it's direction to some extend.
        // bool LineTrace() const;

        GunAnimComp AnimationComp;
        GunStateEnum CurrentGunState{GunStateEnum::Idle};
    };
}
