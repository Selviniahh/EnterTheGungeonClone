#include <imgui-SFML.h>
#include <SFML/Window.hpp>
#include "GameManager.h"
#include "DebugTexts.h"
#include "InputManager.h"
#include "SpriteBatch.h"
#include "Globals.h"
#include "../Core/Scene/Scene.h"
#include "../Characters/Hero.h"
#include "../UI/UserInterface.h"

sf::Event ETG::GameManager::GameEvent{};
using namespace ETG::Globals;

ETG::GameManager::~GameManager() = default;

ETG::GameManager::GameManager()
{
    Initialize();
}

void ETG::GameManager::Initialize()
{
    //During development for different resolution and size monitors, Window mode will be half of host's window size
    const auto VideoMode = sf::VideoMode::getDesktopMode();
    Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(VideoMode.width / 1.5, VideoMode.height / 1.5), "SFML example");
    // Window = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "SFML example", sf::Style::Fullscreen);
    Window->requestFocus();
    Window->setFramerateLimit(Globals::FPS);

    //Initialize GameState instance before anything and initialize SceneObj vector
    GameState::GetInstance();
    GameState::GetInstance().SetSceneObjs(SceneObjects);

    //What's going on at here is only applicable for Scene object. 
    Scene = std::make_unique<class Scene>();
    Scene->SetObjectNameToSelfClassName();
    GameState::GetInstance().SetSceneObj(Scene.get());


    //NOTE: Secondly EngineUI needs to be initialized
    EngineUI.Initialize();
    
    Globals::Initialize(Window);
    InputManager::InitializeDebugText();

    UI = ETG::CreateGameObjectDefault<UserInterface>();
    UI->Initialize();
    
    Hero = ETG::CreateGameObjectDefault<class Hero>(sf::Vector2f{10,10});

    DebugText = std::make_unique<class DebugText>();
    
    //TODO: Work on safely destroying and error resolution for accessing destroyed object
    // DestroyGameObject(Hero);

    
}

void ETG::GameManager::Update()
{
    if (HasFocus)
    {
        EngineUI.Update();
        Globals::Update();
        InputManager::Update();
        Hero->Update();
        UI->Update();
    }

}

void ETG::GameManager::Draw()
{
    Window->clear(sf::Color::Black);

    //NOTE: Draw the main game scene with Custom view. These draws will be drawn zoomed
    Window->setView(Globals::MainView);

    ETG::GlobSpriteBatch.begin();
    Hero->Draw();
    ETG::GlobSpriteBatch.end(*Window);

    //NOTE: Switch to the default (un-zoomed) view for overlays (UI). These draws will be drawn in screen coords.
    //NOTE: Which means, even though The view zoomed or moved, these draws will always stay persistent in initial given coords
    Window->setView(Window->getDefaultView());

    ETG::GlobSpriteBatch.begin();
    UI->Draw();
    ETG::GlobSpriteBatch.end(*Window);

    //NOTE: non batch draws here. 
    DebugText->Draw(*Window);

    EngineUI.Draw();

    //Display the frame after everything is set to be drawn
    Window->display();
}

void ETG::GameManager::ProcessEvents()
{
    while (Window->pollEvent(GameEvent))
    {
        if (GameEvent.type == sf::Event::LostFocus) HasFocus = false;
        if (GameEvent.type == sf::Event::GainedFocus) HasFocus = true;
        if (GameEvent.type == sf::Event::Closed)
        {
            Font.reset();
            Window->close();
            return;
        }

        // Handle window resize
        // Inside GameManager::ProcessEvents
        if (GameEvent.type == sf::Event::Resized)
        {
            // Update the global screen size
            ScreenSize = {GameEvent.size.width, GameEvent.size.height};

            // Optionally update the default view if you rely on it
            sf::View defaultView(sf::FloatRect(0.f, 0.f, GameEvent.size.width, GameEvent.size.height));
            Window->setView(defaultView);

            // Recalculate UI positions based on the new screen size.
            // This could be done by calling a dedicated update method in your UI.
            UI->Initialize(); // or UI.UpdatePositions(); if you separate the logic.
        }

        //Poll and process events for ImGUI
        ImGui::SFML::ProcessEvent(*Window, GameEvent);
    }
}
