#pragma once
#include <boost/describe/class.hpp>
#include "../../Components/EnemyMoveCompBase.h"

namespace ETG
{
    class BulletMan;

    class BulletManMoveComp : public EnemyMoveCompBase
    {
    public:
        BulletManMoveComp();
        ~BulletManMoveComp() override = default;

        void Initialize() override;

    protected:
        // Override the AI movement behavior for BulletMan
        void UpdateAIMovement() override;
        
    private:
        // Specific reference to BulletMan owner
        BulletMan* BulletManPtr = nullptr;

        BOOST_DESCRIBE_CLASS(BulletManMoveComp, (EnemyMoveCompBase),
                             (BulletManPtr), (), ())
    };
}
