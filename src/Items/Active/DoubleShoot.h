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
        std::string GetModifierName() const override { return "MultiShot"; }

    private:
        int ShotCount;
    };
    
    class Hero;
    class CollisionComponent;

    class DoubleShoot : public ActiveItemBase
    {
    public:
        DoubleShoot();
        ~DoubleShoot() override = default;

        void Initialize() override;
        void Update() override;
        void Draw() override;
        static void Perk(const Hero* hero);

        std::unique_ptr<CollisionComponent> CollisionComp;
    };
}

