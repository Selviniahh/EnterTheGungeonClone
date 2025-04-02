#include <imgui-SFML.h>
#include <SFML/Window.hpp>
#include "GameManager.h"
#include "DebugTexts.h"
#include "InputManager.h"
#include "SpriteBatch.h"
#include "Globals.h"
#include "../Guns/AK-47/AK47.h"
#include "../Core/Components/CollisionComponent.h"
#include "../Core/Scene/Scene.h"
#include "../Characters/Hero.h"
#include "../UI/UserInterface.h"
#include "../Enemy/BulletMan/BulletMan.h"
#include "../Items/Active/DoubleShoot.h"
#include "../Items/Passive/PlatinumBullets.h"
#include "../Guns/SawedOff/SawedOff.h"
#include "../Guns/Magnum/Magnum.h"


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
    Window = std::make_shared<sf::RenderWindow>(sf::VideoMode(VideoMode.width / 1.4, VideoMode.height / 1.4), "SFML example");
    // Window = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "SFML example", sf::Style::Fullscreen);
    Window->requestFocus();
    Window->setFramerateLimit(Globals::FPS);
    GameState::GetInstance().SetRenderWindow(Window.get());

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
    
    Hero = ETG::CreateGameObjectDefault<class Hero>(sf::Vector2f{10,10});

    UI = ETG::CreateGameObjectDefault<UserInterface>();
    
    //Always initialize debug text last 
    DebugText = std::make_unique<class DebugText>();

    BulletMan = ETG::CreateGameObjectDefault<class BulletMan>(sf::Vector2f{50,50});
    PlatinumBullets = ETG::CreateGameObjectDefault<class PlatinumBullets>();
    DoubleShoot = ETG::CreateGameObjectDefault<class DoubleShoot>();
    Ak47 = ETG::CreateGameObjectDefault<class AK47>(sf::Vector2f{-100,100});
    SawedOff = ETG::CreateGameObjectDefault<class SawedOff>(sf::Vector2f{-150,100});
    Magnum = ETG::CreateGameObjectDefault<class Magnum>(sf::Vector2f{-200,100});


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
        Ak47->Update();
        SawedOff->Update();
        Magnum->Update();
        BulletMan->Update();
        PlatinumBullets->Update();
        DoubleShoot->Update();
    }

}

void ETG::GameManager::Draw()
{
    if (!HasFocus) return;
    Window->clear({7,255,255,255});

    //NOTE: Draw the main game scene with Custom view. These draws will be drawn zoomed
    Window->setView(Globals::MainView);

    GlobSpriteBatch.begin();
    Hero->Draw();
    BulletMan->Draw();
    PlatinumBullets->Draw();
    DoubleShoot->Draw();
    Ak47->Draw();
    SawedOff->Draw();
    Magnum->Draw();
    GlobSpriteBatch.end(*Window);

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

            //TODO: When the window is resized, I need to recalculate every position of every UI object.
            // Recalculate UI positions based on the new screen size.
            // UI->Initialize(); // or UI.UpdatePositions(); if you separate the logic.
        }

        //Poll and process events for ImGUI
        ImGui::SFML::ProcessEvent(*Window, GameEvent);
    }
}
