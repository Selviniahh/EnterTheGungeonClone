#pragma once
#include "ActiveItemBase.h"
#include "../../Guns/Base/GunModifiers.h"

namespace ETG
{
    //Modifier that changes number of bullets fired per shot
    class MultiShotModifier : public GunModifier
    {
    public:
        explicit MultiShotModifier(const int shotCount) : ShotCount(shotCount)
        {
        }

        int GetShotCount() const { return ShotCount; }
        int GetSpread() const { return Spread; }
        std::string GetModifierName() const override { return "MultiShot"; }
        

    private:
        int ShotCount;
        int Spread = 0;
    };
    
    class Hero;
    class CollisionComponent;

    class DoubleShoot : public ActiveItemBase
    {
    public:
        DoubleShoot();
        ~DoubleShoot() override = default;
        void RequestUsage() override;

        void Initialize() override;
        void Update() override;
        void Draw() override;
        static void ApplyPerk(const Hero* hero);

        std::unique_ptr<CollisionComponent> CollisionComp;
        static float GetDefaultCooldown() { return 15.f; }
        static float GetDefaultActiveTime() { return 10.f; }
        
    };
}

