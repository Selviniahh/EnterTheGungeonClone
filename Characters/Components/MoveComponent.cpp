#include "MoveComponent.h"
#include "../../Managers/InputManager.h"
#include "../Hero.h"

ETG::MoveComponent::MoveComponent() : Speed(100.f)
{
}

void ETG::MoveComponent::UpdateMovement(Hero& hero) const
{
    if (InputManager::IsMoving() && !hero.IsDashing)
    {
        const sf::Vector2f moveDir = Globals::Normalize(InputManager::direction);
        Hero::HeroPosition += moveDir * Speed * Globals::FrameTick;
        hero.CurrentHeroState = HeroStateEnum::Run;
    }
    else if (hero.IsDashing)
    {
        hero.CurrentHeroState = HeroStateEnum::Dash;
    }
    else
    {
        hero.CurrentHeroState = HeroStateEnum::Idle;
    }
}
