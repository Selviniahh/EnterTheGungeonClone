#include "GameManager.h"
#include "DebugTexts.h"
#include "InputManager.h"

void ETG::GameManager::Initialize()
{
    //During development for different resolution and size monitors, Window mode will be half of host's window size
    const auto VideoMode = sf::VideoMode::getDesktopMode();
    Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(VideoMode.width / 1.5, VideoMode.height / 1.5), "SFML example");
    // Window = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "SFML example", sf::Style::Fullscreen);
    Window->requestFocus();
    Window->setFramerateLimit(Globals::FPS);
    Globals::Initialize(Window);
    InputManager::InitializeDebugText();
    UI.Initialize();
}

void ETG::GameManager::Update()
{
    if (HasFocus)
    {
        Globals::Update();
        InputManager::Update();
        Hero.Update();
    }
}

void ETG::GameManager::Draw()
{
    Window->clear(sf::Color::Black);

    //NOTE: Draw the main game scene with Custom view. These draws will be drawn zoomed
    Window->setView(Globals::MainView);

    SpriteBatch.begin();
    Hero.Draw();
    SpriteBatch.end(*Window);

    //NOTE: Switch to the default (un-zoomed) view for overlays (UI). These draws will be drawn in screen coords.
    //NOTE: Which means, even though The view zoomed or moved, these draws will always stay persistent in initial given coords
    Window->setView(Window->getDefaultView());

    SpriteBatch.begin();
    UI.Draw();
    SpriteBatch.end(*Window);

    //NOTE: non batch draws here. 
    DebugText::Draw(*Window);

    //Display the frame
    Window->display();
}

void ETG::GameManager::ProcessEvents()
{
    sf::Event event{};
    while (Window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) Window->close();
        if (event.type == sf::Event::LostFocus) HasFocus = false;
        if (event.type == sf::Event::GainedFocus) HasFocus = true;
        
        // Handle window resize
        // Inside GameManager::ProcessEvents
        if (event.type == sf::Event::Resized)
        {
            // Update the global screen size
            ScreenSize = { event.size.width, event.size.height };

            // Optionally update the default view if you rely on it
            sf::View defaultView(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
            Window->setView(defaultView);
            
            // Recalculate UI positions based on the new screen size.
            // This could be done by calling a dedicated update method in your UI.
            UI.Initialize(); // or UI.UpdatePositions(); if you separate the logic.
        }
    }
}
