#pragma once
#include "../Hero.h"
#include "../../Core/Components/BaseMoveComp.h"
#include "../../Core/Events/EventDelegate.h"

namespace ETG
{
    class Hero; // Forward-declare the hero.
    enum class HeroDashEnum;

    class HeroMoveComp : public BaseMoveComp
    {
    public:
        HeroMoveComp();

        // Update movement using input.
        void UpdateMovement();

        // Override Update to include our movement update.
        void Update() override;

        // Pointer to the associated Hero (set from GameState or elsewhere).
        Hero* HeroPtr = nullptr;
        BOOST_DESCRIBE_CLASS(HeroMoveComp, (BaseMoveComp), (HeroPtr), (), ())
    };
}
