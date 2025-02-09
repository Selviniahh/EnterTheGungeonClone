#pragma once
#include "../../Core/Components/MoveComp.h"

namespace ETG {
    class Hero;  // Forward-declare the hero.

    class MoveComponent : public MoveComp
    {
    public:
        MoveComponent();

        // Update movement using input.
        void UpdateMovement();

        // Override Update to include our movement update.
        void Update() override;

        // Pointer to the associated Hero (set from GameState or elsewhere).
        Hero* HeroPtr = nullptr;
    };
}
