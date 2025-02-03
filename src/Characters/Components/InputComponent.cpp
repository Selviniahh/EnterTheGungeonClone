#include "InputComponent.h"

#include "../Hero.h"
#include "../../Managers/InputManager.h"

namespace ETG
{
    InputComponent::InputComponent()
    {
        SetRanges();
    }

    void InputComponent::Update(Hero& hero) const
    {
        UpdateDirection(hero);
    }

    void InputComponent::SetRanges()
    {
        DirectionMap[{0, 55}] = Direction::Right;
        DirectionMap[{55, 75}] = Direction::FrontHandRight;
        DirectionMap[{75, 115}] = Direction::FrontHandLeft;
        DirectionMap[{115, 190}] = Direction::Left;
        DirectionMap[{190, 245}] = Direction::BackDiagonalLeft;
        DirectionMap[{245, 265}] = Direction::BackHandLeft;
        DirectionMap[{265, 290}] = Direction::BackHandRight;
        DirectionMap[{290, 310}] = Direction::BackDiagonalRight;
        DirectionMap[{310, 360}] = Direction::Right;
    }

    void InputComponent::UpdateDirection(Hero& hero) const
    {
        // Convert mouse angle to [0..360)
        float angle = Misc::RadiansToDegrees(InputManager::GetMouseAngleRelativeToHero());
        if (angle < 0.f) angle += 360.f;

        // Store on the Hero (or handle in input component)
        hero.MouseAngle = angle;
        hero.CurrentDirection = GetDirectionFromAngle(angle);
    }

    Direction InputComponent::GetDirectionFromAngle(float angle) const
    {
        for (const auto& entry : DirectionMap)
        {
            if (Hero::MouseAngle >= entry.first.first && Hero::MouseAngle < entry.first.second)
            {
                return entry.second;
            }
        }
        throw std::out_of_range("Mouse angle is out of defined ranges");
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
