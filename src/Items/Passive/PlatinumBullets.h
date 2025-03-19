#pragma once
#include <random>

#include "PassiveItemBase.h"
#include "../../Core/Components/CollisionComponent.h" // Include the full definition

namespace ETG
{
    class CollisionComponent;
    class Hero;

    class PlatinumBullets : public PassiveItemBase
    {
    public:
        PlatinumBullets();
        ~PlatinumBullets() override = default;

        void Initialize() override;
        void Update() override;
        void Draw() override;
        static void Perk(const Hero* hero);

        std::unique_ptr<CollisionComponent> CollisionComp;

    private:
        
    };    
}

