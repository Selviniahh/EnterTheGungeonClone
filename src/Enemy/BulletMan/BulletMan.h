#pragma once
#include "../EnemyBase.h"
#include "../../Characters/Hero.h"

namespace ETG
{
    class BulletManAnimComp;
    
    class BulletMan : public EnemyBase
    {
    public:
        explicit BulletMan(const sf::Vector2f& position);
        ~BulletMan() override = default;
        void Initialize() override;
        void Update() override;
        void Draw() override;


        EnemyStateEnum BulletManState{EnemyStateEnum::Idle}; // Initialize with default state
        Direction BulletManDir{Direction::Right}; // Initialize with default direction
        
        std::unique_ptr<BulletManAnimComp> AnimationComp;
    };

    class BulletManAnimComp : public BaseAnimComp<EnemyStateEnum>
    {
    public:
        BulletManAnimComp();
        void SetAnimations() override;
        void Initialize() override;
        void Update() override;

        BulletMan* BulletMan{};
        std::shared_ptr<sf::Texture> Texture; // Add texture field to match BaseAnimComp pattern
    };
}