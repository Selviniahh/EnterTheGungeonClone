#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "../Core/GameObjectBase.h"
#include "../Managers/StateEnums.h"
#include "../Core/Factory.h"
#include "../Guns/Base/GunBase.h"

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

    class Hero : public GameObjectBase
    {
    public:
        explicit Hero(sf::Vector2f Position);
        ~Hero() override;
        void Update() override;
        void Initialize() override;
        void Draw() override;
        [[nodiscard]] GunBase* GetCurrentHoldingGun() const;

        static float MouseAngle;
        static Direction CurrentDirection;
        static bool IsShooting;

        HeroStateEnum CurrentHeroState{HeroStateEnum::Idle};

        std::unique_ptr<RogueSpecial> RogueSpecial;
        std::unique_ptr<HeroMoveComp> MoveComp;
        std::unique_ptr<Hand> Hand;
        std::unique_ptr<ReloadText> ReloadText;
        std::unique_ptr<CollisionComponent> CollisionComp;

        ActiveItemBase* CurrActiveItem{};

        std::unique_ptr<HeroAnimComp> AnimationComp;
        std::unique_ptr<InputComponent> InputComp;

        //Selected guns 
        std::vector<GunBase*> EquippedGuns; // Array of equipped guns
        GunBase* CurrentGun = nullptr; // Currently selected gun
        int currentGunIndex = 0; // Track the index of current gun

        //When equipping a new gun pickup
        void EquipGun(GunBase* newGun)
        {
            EquippedGuns.push_back(newGun);
            CurrentGun = newGun; // Set the new gun as the current one by default
            currentGunIndex = EquippedGuns.size() - 1;
        }

        // When scrolling the mouse wheel, switch back to the default (index 0) gun.
        void SwitchToPreviousGun()
        {
            // First check if we have any guns at all
            if (EquippedGuns.empty()) return;
    
            // Move index backwards with wraparound
            // No need for additional bounds check - the modulo operation guarantees the index is valid if the vector is not empty
            currentGunIndex = (currentGunIndex - 1 + EquippedGuns.size()) % EquippedGuns.size();
            CurrentGun = EquippedGuns[currentGunIndex];
        }
        void SwitchNextGun()
        {
            if (EquippedGuns.empty()) return;
    
            // Move index forwards with wraparound
            currentGunIndex = (currentGunIndex + 1) % EquippedGuns.size();
            CurrentGun = EquippedGuns[currentGunIndex];
        }

        BOOST_DESCRIBE_CLASS(Hero, (GameObjectBase),
                             (MouseAngle, CurrentDirection, CurrentHeroState, IsShooting),
                             (),
                             ())
    };
}
