#pragma once
#include "InputManager.h"
#include "GameState.h"

namespace ETG
{
    class DebugText
    {
    public:
        Hero* HeroPtr = nullptr;

        void Draw(sf::RenderWindow& window)
        {
            if (!HeroPtr) HeroPtr = GameState::GetInstance().GetHero();

            // Reset textPos to starting position each frame
            InputManager::textPos = {0.f, -20.f};

            // Draw debug information
            DrawDebugText("Direction: " + std::to_string(InputManager::direction.x) + ", " + std::to_string(InputManager::direction.y), window);

            //Mouse position that my monitor's top left point will be (0,0)
            DrawDebugText("Screen Mouse Position: " + std::to_string(sf::Mouse::getPosition().x) + ", " + std::to_string(sf::Mouse::getPosition().y), window);

            //NOTE: IMPORTANT!!! Mouse position relative to view. This is so important because when hero rotating around mouse, if view zoomed or moved, we need to take View into account
            DrawDebugText("View Relative Mouse world Position: " + std::to_string(InputManager::ViewLocalMousePos.x) + " " + std::to_string(InputManager::ViewLocalMousePos.y), window);

            const sf::Vector2f diff = InputManager::WorldMousePos - HeroPtr->GetPosition();
            DrawDebugText(" Hero - Mouse pos: " + std::to_string(diff.x) + " " + std::to_string(diff.y), window);
            DrawDebugText(" Mouse angle: " + std::to_string(Hero::MouseAngle), window);

            //Represents the mouse position as if the View were neither zoomed nor moved
            DrawDebugText("View ignored relative Mouse world Position: " + std::to_string(InputManager::WorldMousePos.x) + " " + std::to_string(InputManager::WorldMousePos.y), window);

            //Mouse Relative to hero
            DrawDebugText("Mouse Relative to Hero: " + std::to_string(sf::Mouse::getPosition().x - HeroPtr->GetPosition().x) + ", " + std::to_string(sf::Mouse::getPosition().y - HeroPtr->GetPosition().y), window);

            // Moving state
            DrawDebugText("Moving: " + std::string(InputManager::IsMoving() ? "true" : "false"), window);
            DrawDebugText("WindowSize: " + std::to_string(Globals::ScreenSize.x) + " " + std::to_string(Globals::ScreenSize.y), window);

            DrawDebugText("FPS: " + std::to_string(1 / Globals::FrameTick), window);
            DrawDebugText("Hero position: " + std::to_string(HeroPtr->GetPosition().x) + " " + std::to_string(HeroPtr->GetPosition().y), window);
            DrawDebugText("Zoom Scale: " + std::to_string(InputManager::ZoomScale), window);

            // const sf::Vector2f viewTopLeft = Globals::MainView.getCenter() - (Globals::MainView.getSize() / 2.0f);
            DrawDebugText("View Center: " + std::to_string(Globals::MainView.getCenter().x) + " " + std::to_string(Globals::MainView.getCenter().y), window);
            DrawDebugText("View Size: " + std::to_string(Globals::MainView.getSize().x) + " " + std::to_string(Globals::MainView.getSize().y), window);
            DrawDebugText("CurrentDirection: " + DirectionUtils::StringifyDirection(Hero::CurrentDirection), window);
            DrawDebugText(std::string("Is Shooting: ") + (Hero::IsShooting ? "True" : "False"), window);
            DrawDebugText("Hero Velocity: " + std::to_string(HeroPtr->MoveComp.Velocity.x) + " " + std::to_string(HeroPtr->MoveComp.Velocity.y), window);
            
        }

        static void DrawDebugText(const std::string& str, sf::RenderWindow& window)
        {
            InputManager::debugText.setString(str);
            InputManager::debugText.setPosition(SetDebugTextPos(InputManager::textPos));
            window.draw(InputManager::debugText);
        }

        static sf::Vector2f& SetDebugTextPos(sf::Vector2f& pos)
        {
            pos.y += 20.f; // Increment Y position for next line
            return pos;
        }
    };
}
