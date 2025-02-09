#include "InputComponent.h"

#include "../Hero.h"
#include "../../Managers/InputManager.h"
#include "../../Utils/DirectionUtils.h"
#include "../../Utils/Math.h"

namespace ETG
{
    InputComponent::InputComponent()
    {
        DirectionUtils::PopulateDirectionRanges(DirectionMap);
    }

    void InputComponent::Update(Hero& hero) const
    {
        UpdateDirection(hero);
    }
    

    void InputComponent::UpdateDirection(Hero& hero) const
    {
        // Convert mouse angle to [0..360)
        float angle = Math::RadiansToDegrees(InputManager::GetMouseAngleRelativeToHero());
        if (angle < 0.f) angle += 360.f;

        // Store on the Hero (or handle in input component)
        hero.MouseAngle = angle;
        hero.CurrentDirection = DirectionUtils::GetDirectionFromAngle(DirectionMap, angle);
    }

    DashEnum InputComponent::GetDashDirectionEnum()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) return DashEnum::Dash_BackWard;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) return DashEnum::Dash_BackWard;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) return DashEnum::Dash_BackWard;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) return DashEnum::Dash_BackWard;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) return DashEnum::Dash_Left;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) return DashEnum::Dash_Right;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) return DashEnum::Dash_Back;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) return DashEnum::Dash_Front;
        return DashEnum::Unknown;
    }
}
