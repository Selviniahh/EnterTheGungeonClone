#include "InputManager.h"

#include "GameManager.h"
#include "GameState.h"
#include "../Engine/Engine.h"

void ETG::InputManager::Update()
{
    if (!HeroPtr) HeroPtr = GameState::GetInstance().GetHero();

    if (!Engine::IsGameWindowFocused()) return;


    // Update PreviousGameFocus at the end of the frame to track state correctly
    Engine::PreviousGameFocus = Engine::CurrentGameFocus;

    ZoomScale = GetZoomScale(Globals::MainView, *Globals::Window);

    const float adjustedZoomFactor = AdjustZoomFactor();
    const float adjustedMoveFactor = AdjustMoveFactor();

    //Calculate directions. It can only be -1 or 1 
    direction = sf::Vector2f(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) direction.x--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) direction.x++;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) direction.y--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) direction.y++;

    //shooting
    Hero::IsShooting = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    //Camera Effects:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) Globals::MainView.zoom(1.0f - adjustedZoomFactor);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) Globals::MainView.zoom(1.0f + adjustedZoomFactor);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) Globals::MainView.move(0, -adjustedMoveFactor);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) Globals::MainView.move(0, +adjustedMoveFactor);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) Globals::MainView.move(+adjustedMoveFactor, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) Globals::MainView.move(-adjustedMoveFactor, 0);

    ViewLocalMousePos = GetRelativeMousePos();
    WorldMousePos = Globals::Window->mapPixelToCoords(sf::Mouse::getPosition(*Globals::Window), Globals::MainView);
}


void ETG::InputManager::InitializeDebugText()
{
    debugText.setFont(Globals::Font);
    debugText.setCharacterSize(16);
    debugText.setFillColor(sf::Color::Yellow);
}

float ETG::InputManager::GetZoomScale(const sf::View& currentView, const sf::RenderWindow& window)
{
    //Default view size
    sf::Vector2f defSize = window.getDefaultView().getSize();

    //Get current view size
    sf::Vector2f currSize = currentView.getSize();

    return defSize.x / currSize.x;
}

float ETG::InputManager::AdjustMoveFactor()
{
    const float scaleRatio = 10000.f / ZoomScale;
    float adjustedMoveFactor = ZoomFactor * std::sqrt(scaleRatio);

    adjustedMoveFactor = std::clamp(adjustedMoveFactor, MinMoveSpeed, MaxMoveSpeed);
    return adjustedMoveFactor;
}

float ETG::InputManager::AdjustZoomFactor()
{
    const float scaleRatio = 0.1f / ZoomScale;
    float adjustedZoomFactor = ZoomFactor * std::sqrt(scaleRatio);
    adjustedZoomFactor = std::clamp(adjustedZoomFactor, MinScaleSpeed, MaxScaleSpeed);
    return adjustedZoomFactor;
}

float ETG::InputManager::GetMouseAngleRelativeToHero()
{
    const sf::Vector2f diff = WorldMousePos - HeroPtr->GetPosition();
    return std::atan2(diff.y, diff.x);
}

sf::Vector2f ETG::InputManager::GetRelativeMousePos()
{
    //Mouse world position
    const sf::Vector2f MousePos = Globals::Window->mapPixelToCoords(sf::Mouse::getPosition(*Globals::Window), Globals::MainView);

    // Calculate the top-left corner of the view in world coordinates
    const sf::Vector2f viewTopLeft = Globals::MainView.getCenter() - (Globals::MainView.getSize() / 2.0f);

    // Subtract the view's top-left to get relative mouse position
    return MousePos - viewTopLeft;
}
