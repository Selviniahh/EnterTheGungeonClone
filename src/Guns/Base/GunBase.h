#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GunBase.h"
#include "../../Animation/Animation.h"
#include "../../Core/GameObject.h"
#include "../../Managers/StateEnums.h"
#include "../../Core/Components/BaseAnimComp.h"

class ProjectileBase;

namespace ETG
{
    enum class GunStateEnum;
    
    class GunBase : public GameObject
    {
    public:
        explicit GunBase(sf::Vector2f Position, float pressTime, float velocity, float maxProjectileRange, float timerForVelocity);
        ~GunBase() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;
        virtual void Shoot();

        using GameObject::Rotation; //Make Rotation public in Gunbase

    protected:
        // Rotates an offset vector according to the gun's current rotation.
        sf::Vector2f RotateVector(const sf::Vector2f& offset) const;

        std::vector<std::unique_ptr<ProjectileBase>> projectiles;
        std::shared_ptr<sf::Texture> GunTexture;
        std::shared_ptr<sf::Texture> ProjTexture;

        // Muzzle flash variables (instance sets up its own animation).
        Animation muzzleFlashAnim;
        float MuzzleFlashEachFrameSpeed;
        sf::Vector2f MuzzleFlashPos;
        sf::Vector2f MuzzleFlashOffset;

        float pressTime;
        float velocity;
        float maxProjectileRange;
        float timerForVelocity;
        bool isAttacking{};

        // Origin offset for spawning projectiles.
        sf::Vector2f OriginOffset;

        // Arrow variables (common to all guns). Arrow will be disabled in Release
        std::shared_ptr<sf::Texture> ArrowTex;
        sf::Vector2f arrowOffset;
        sf::Vector2f arrowOrigin;
        sf::Vector2f arrowOriginOffset;
        sf::Vector2f arrowPos;

        //Gun Animation
        std::unique_ptr<BaseAnimComp<GunStateEnum>> AnimationComp;
        GunStateEnum CurrentGunState{GunStateEnum::Idle};
    };
}