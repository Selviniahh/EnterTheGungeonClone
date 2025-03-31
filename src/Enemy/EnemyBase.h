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
        std::unique_ptr<BaseHealthComp> HealthComp;
        std::unique_ptr<Hand> Hand;
        Hero* Hero;

        BOOST_DESCRIBE_CLASS(EnemyBase, (GameObjectBase),
            (Hero), (), ())
    };
}
