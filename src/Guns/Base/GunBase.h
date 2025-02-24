#pragma once
#include "../../Animation/Animation.h"
#include "../../Core/GameObject.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../Managers/StateEnums.h"
#include "GunBase.h"
#include "../../Projectile/ProjectileBase.h"
#include "../../Managers/SpriteBatch.h"

class ProjectileBase;

namespace ETG
{
    enum class GunStateEnum;

    template <typename DerivedGunName, typename AnimCompType>
    class GunBase : public GameObject<DerivedGunName>
    {
    public:
        explicit GunBase(sf::Vector2f Position, float pressTime, float velocity, float maxProjectileRange, float timerForVelocity);
        ~GunBase() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;
        virtual void Shoot();

        using GameObject<DerivedGunName>::Rotation; //Make Rotation public in Gunbase

    protected:
        // Rotates an offset vector according to the gun's current rotation.
        sf::Vector2f RotateVector(const sf::Vector2f& offset) const;

        std::vector<ProjectileBase*> projectiles;
        sf::Texture GunTexture;
        sf::Texture ProjTexture;

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
        sf::Texture ArrowTex;
        sf::Vector2f arrowOffset;
        sf::Vector2f arrowOrigin;
        sf::Vector2f arrowOriginOffset;
        sf::Vector2f arrowPos;

        //Gun Animation

        std::unique_ptr<AnimCompType> AnimationComp;
        GunStateEnum CurrentGunState{GunStateEnum::Idle};
    };

    template <typename DerivedGun, typename AnimCompType>
    GunBase<DerivedGun, AnimCompType>::GunBase(const sf::Vector2f Position, const float pressTime, const float velocity, const float maxProjectileRange, const float timerForVelocity)
        : pressTime(pressTime), velocity(velocity), maxProjectileRange(maxProjectileRange), timerForVelocity(timerForVelocity)
    {
        // Initialize common position.
        this->Position = Position;
    }

    using namespace ETG;


    template <typename DerivedGun, typename AnimCompType>
    GunBase<DerivedGun, AnimCompType>::~GunBase()
    {
        for (const auto proj : projectiles)
            delete proj;
    }

    template <typename DerivedGun, typename AnimCompType>
    void GunBase<DerivedGun, AnimCompType>::Initialize()
    {
        // Set the origin based on the current gun texture.
        this->Origin = {
            static_cast<float>(AnimationComp->CurrentTex.getSize().x / 2),
            static_cast<float>(AnimationComp->CurrentTex.getSize().y / 2)
        };
        this->Origin += OriginOffset;

        // Load the arrow texture (common for all guns).
        const std::filesystem::path arrowPath = std::filesystem::path(RESOURCE_PATH) / "Projectiles" / "Arrow.png";
        if (!ArrowTex.loadFromFile(arrowPath.generic_string()))
            throw std::runtime_error(arrowPath.generic_string() + " not found");


        arrowOrigin = {
            static_cast<float>(ArrowTex.getSize().x / 2),
            static_cast<float>(ArrowTex.getSize().y / 2)
        };
        arrowOrigin += arrowOriginOffset;

        // Muzzle flash animation should be set up by the derived class.
        muzzleFlashAnim.Active = false;
        this->Depth = 2;
    }

    template <typename DerivedGun, typename AnimCompType>
    void GunBase<DerivedGun, AnimCompType>::Update()
    {
        GameObject<DerivedGun>::Update();

        timerForVelocity += Globals::FrameTick;

        // If the shoot animation finished, revert to idle.
        if (AnimationComp->CurrentState == GunStateEnum::Shoot &&
            AnimationComp->AnimManagerDict[AnimationComp->CurrentState].IsAnimationFinished())
        {
            CurrentGunState = GunStateEnum::Idle;
        }

        // Update arrow position.
        arrowPos = this->Position + RotateVector(arrowOffset);

        // Update gun animation.
        AnimationComp->Update(CurrentGunState, CurrentGunState);
        GunTexture = AnimationComp->CurrentTex;

        // Update muzzle flash position and animation.
        MuzzleFlashPos = arrowPos + RotateVector(MuzzleFlashOffset);
        muzzleFlashAnim.Update();
        if (muzzleFlashAnim.Active && muzzleFlashAnim.IsAnimationFinished())
            muzzleFlashAnim.Active = false;

        // Update projectiles.
        for (const auto proj : projectiles)
        {
            proj->Update();
        }
    }

    template <typename DerivedGun, typename AnimCompType>
    void GunBase<DerivedGun, AnimCompType>::Draw()
    {
        // Draw the gun.
        const auto& DrawProps = this->GetDrawProperties();
        AnimationComp->Draw(DrawProps.Position, DrawProps.Origin, DrawProps.Scale, DrawProps.Rotation, DrawProps.Depth);
        Globals::DrawSinglePixelAtLoc(DrawProps.Position, {1, 1}, DrawProps.Rotation);

        // Draw the arrow representation.
        SpriteBatch::SimpleDraw(ArrowTex, arrowPos, DrawProps.Rotation, arrowOrigin);
        Globals::DrawSinglePixelAtLoc(arrowPos, {1, 1}, DrawProps.Rotation);

        // Draw projectiles.
        for (const auto proj : projectiles)
        {
            proj->Draw();
        }

        // Draw the muzzle flash.
        //NOTE: Muzzle flash did not constructed as drawable object yet. So all the properties are arbitrarily created.

        muzzleFlashAnim.Draw(muzzleFlashAnim.Texture, MuzzleFlashPos, sf::Color::White, DrawProps.Rotation, muzzleFlashAnim.Origin, {1, 1}, DrawProps.Depth);
    }

    template <typename DerivedGun, typename AnimCompType>
    void GunBase<DerivedGun, AnimCompType>::Shoot()
    {
        if (timerForVelocity >= pressTime)
        {
            //Set muzzleFlashAnim Active to true. Once the animation is drawn, it will be set back to false. 
            muzzleFlashAnim.Active = true;

            //Set animation to Shoot
            CurrentGunState = GunStateEnum::Shoot;
            timerForVelocity = 0;

            // Restart shoot animation.
            AnimationComp->AnimManagerDict[GunStateEnum::Shoot].AnimationDict[GunStateEnum::Shoot].Restart();

            // Calculate projectile velocity.
            const sf::Vector2f spawnPos = arrowPos;
            const float angle = Rotation;
            const float rad = angle * std::numbers::pi / 180.f;
            const sf::Vector2f direction(std::cos(rad), std::sin(rad));
            const sf::Vector2f projVelocity = direction * velocity;

            // Spawn projectile.
            auto* proj = new ProjectileBase(ProjTexture, spawnPos, projVelocity, maxProjectileRange, Rotation);
            projectiles.push_back(proj);

            // Restart muzzle flash animation.
            muzzleFlashAnim.Restart();
        }
    }

    template <typename DerivedGun, typename AnimCompType>
    sf::Vector2f GunBase<DerivedGun, AnimCompType>::RotateVector(const sf::Vector2f& offset) const
    {
        const float angleRad = Rotation * (std::numbers::pi / 180.f);
        sf::Vector2f scaledOffset(offset.x * this->Scale.x, offset.y * this->Scale.y);

        return {
            scaledOffset.x * std::cos(angleRad) - scaledOffset.y * std::sin(angleRad),
            scaledOffset.x * std::sin(angleRad) + scaledOffset.y * std::cos(angleRad)
        };
    }
}
