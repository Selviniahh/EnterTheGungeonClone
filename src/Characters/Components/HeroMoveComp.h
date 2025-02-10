#pragma once
#include "../../Core/Components/BaseMoveComp.h"

namespace ETG {
    class Hero;  // Forward-declare the hero.

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
    };
}
