#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "../Core/GameObjectBase.h"
#include "../Managers/Enum/StateEnums.h"
#include "../Core/Factory.h"
#include "../Guns/Base/GunBase.h"
#include "../Managers/Enum/StateFlags.h"

namespace ETG
{
    class CollisionComponent;
    class ReloadText;
    class GunBase;
    class Hand;
    class RogueSpecial;
    class HeroAnimComp;
    class InputComponent;
    class HeroMoveComp;
    class ReloadSlider;
    class BaseHealthComp;

    class Hero : public GameObjectBase
    {
    public:
        explicit Hero(sf::Vector2f Position);
        ~Hero() override;
        void UpdateComponents();
        void UpdateAnimations();
        void UpdateHand() const;
        void UpdateGuns() const;
        void HandleShooting() const;
        void HandleActiveItem() const;

        void Update() override;
        void Initialize() override;
        void Draw() override;
        [[nodiscard]] GunBase* GetCurrentHoldingGun() const;

    public:
        static float MouseAngle;
        static Direction CurrentDirection;
        static bool IsShooting;

        HeroStateFlags StateFlags{HeroStateFlags::StateIdle};

        std::unique_ptr<RogueSpecial> RogueSpecial;
        std::unique_ptr<HeroMoveComp> MoveComp;
        std::unique_ptr<Hand> Hand;
        std::unique_ptr<ReloadText> ReloadText;
        std::unique_ptr<CollisionComponent> CollisionComp;
        std::unique_ptr<BaseHealthComp> HealthComp;

        ActiveItemBase* CurrActiveItem{};

        std::unique_ptr<HeroAnimComp> AnimationComp;
        std::unique_ptr<InputComponent> InputComp;

        //Selected guns 
        std::vector<GunBase*> EquippedGuns; // Array of equipped guns
        GunBase* CurrentGun = nullptr; // Currently selected gun
        int currentGunIndex = 0; // Track the index of current gun
        float HitKnockBackMagnitude = 150.f;
        float EnemyCollideKnockBackMag = 350.f;

        //Helper methods for state management
        void SetState(const HeroStateEnum& state);
        [[nodiscard]] HeroStateEnum GetState() const { return CurrentHeroState; }

        [[nodiscard]] inline bool CanSwitchGuns() const { return !CurrentGun->IsReloading && !HasAnyFlag(StateFlags, HeroStateFlags::PreventGunSwitching); }
        [[nodiscard]] inline bool CanMove() const { return !HasAnyFlag(StateFlags, HeroStateFlags::PreventMovement); }
        [[nodiscard]] inline bool CanShoot() const { return !HasAnyFlag(StateFlags, HeroStateFlags::PreventShooting); }
        [[nodiscard]] inline bool CanFlipAnims() const { return !HasAnyFlag(StateFlags, HeroStateFlags::PreventAnimFlip); }
        [[nodiscard]] inline bool CanUseActiveItems() const { return !HasAnyFlag(StateFlags, HeroStateFlags::PreventActiveItemUsage); }

        //When equipping a new gun pickup
        void EquipGun(GunBase* newGun);
        void SwitchGun(const int& index);

        // When scrolling the mouse wheel, switch back to the default (index 0) gun.
        void SwitchToPreviousGun();
        void SwitchToNextGun();

        BOOST_DESCRIBE_CLASS(Hero, (GameObjectBase),
                             (MouseAngle, CurrentDirection, CurrentHeroState, IsShooting, HitKnockBackMagnitude),
                             (),
                             ())

    private:
        void UpdateGunVisibility() const;
        HeroStateEnum CurrentHeroState{HeroStateEnum::Idle};
    };
}
