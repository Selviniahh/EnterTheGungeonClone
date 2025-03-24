#pragma once
#include "ActiveItemBase.h"

namespace ETG
{
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

        static int GetShootCount() {return 2;}
        static float GetSpread() {return 1;}
        
    };
}

