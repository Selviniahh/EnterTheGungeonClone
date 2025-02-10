#include "HeroMoveComp.h"
#include "../../Managers/InputManager.h"
#include "../../Managers/GameState.h"
#include "../../Utils/Math.h"
#include "../Hero.h"  // For Hero

namespace ETG {

    HeroMoveComp::HeroMoveComp() : BaseMoveComp(200.f, 8000.f, 8000.f) // Adjust parameters as needed.
    {
    }

    void HeroMoveComp::Update()
    {
        // Call the base update (it doing nothing for now)
        BaseMoveComp::Update();
        
        // Then update movement based on current input.
        UpdateMovement();
    }

    void HeroMoveComp::UpdateMovement()
    {
        // Ensure we have a valid hero pointer.
        if (!HeroPtr) HeroPtr = GameState::GetInstance().GetHero();

        // Determine input direction.
        sf::Vector2f inputDir(0.f, 0.f);
        if (InputManager::IsMoving() && !HeroPtr->IsDashing)
        {
            inputDir = InputManager::direction;
            HeroPtr->CurrentHeroState = HeroStateEnum::Run;
        }
        else if (HeroPtr->IsDashing)
        {
            HeroPtr->CurrentHeroState = HeroStateEnum::Dash;
        }
        else
        {
            HeroPtr->CurrentHeroState = HeroStateEnum::Idle;
        }

        // Use the base helper to update velocity and position.
        BaseMoveComp::UpdateMovement(inputDir, HeroPtr->GetPosition());
    }
}
