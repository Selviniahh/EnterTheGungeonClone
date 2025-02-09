#include "MoveComponent.h"
#include "../../Managers/InputManager.h"
#include "../../Managers/GameState.h"
#include "../../Utils/Math.h"
#include "../Hero.h"  // For Hero

namespace ETG {

    MoveComponent::MoveComponent() : MoveComp(200.f, 200.f, 800.f) // Adjust parameters as needed.
    {
    }

    void MoveComponent::Update()
    {
        // Call the base update (it doing nothing for now)
        MoveComp::Update();
        
        // Then update movement based on current input.
        UpdateMovement();
    }

    void MoveComponent::UpdateMovement()
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
        // Assumes Hero has a method GetPosition() returning a reference to its position.
        MoveComp::UpdateMovement(inputDir, HeroPtr->GetPosition());
    }
}
