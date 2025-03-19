#pragma once
#include "../Core/GameObjectBase.h"
#include "../Core/Components/BaseAnimComp.h"
#include "../Managers/StateEnums.h"


// Forward declarations
class BaseHealthComp;
class CollisionComponent;

namespace ETG

// Forward declarations
{
    class Hero;
    class Hand;
    class RogueSpecial;

    class EnemyBase : public GameObjectBase
    {
    protected:
        EnemyBase(); // Add default constructor
        ~EnemyBase() override;
        void Initialize() override;
        void Update() override;
        void Draw() override;

    public:

    protected:
        std::unique_ptr<RogueSpecial> RogueSpecial;
        std::unique_ptr<BaseHealthComp> HealthComp;
        // std::unique_ptr<BaseAnimComp<EnemyStateEnum>> AnimationComp;
        std::unique_ptr<Hand> Hand;
        Hero* Hero;

        float timer{};
        float secTimer{};
    };
}
