#include "GameManager.h"
#include "DebugTexts.h"
#include "InputManager.h"

void ETG::GameManager::Initialize()
{
    Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080), "SFML example");
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

    //Draw the main game scene with Custom view. These draws will be drawn zoomed
    Window->setView(Globals::MainView);

    Hero.Draw();

    //Switch to the default (un-zoomed) view for overlays. These draws will be drawn in screen coords.
    //Which means, even though The view zoomed or moved, these draws will always stay persistent in initial given coords
    Window->setView(Window->getDefaultView());
    UI.Draw();

    //Debug texts will be drawn in screen coords. 
    DebugText::Draw(*Window);
    // DebugText::DrawDebugText(Hero.DebugRectString, *Window);

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
    }
}
