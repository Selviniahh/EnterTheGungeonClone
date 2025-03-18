#include "UserInterface.h"
#include <filesystem>
#include "../Characters/Hero.h"
#include "../Guns/Base/GunBase.h"
#include "../Managers/GameState.h"
#include "../Managers/Globals.h"
#include "../Managers/SpriteBatch.h"
#include "../Core/Factory.h"
#include "UIObjects/AmmoCounter.h"

namespace ETG
{
    UserInterface::UserInterface()
    {
        UserInterface::Initialize();
        GameObjectBase::Initialize();
    }

    void UserInterface::Initialize()
    {
        if (!hero) hero = GameState::GetInstance().GetHero();
        CurrentGun = hero->GetCurrentHoldingGun();
        if (!CurrentGun) throw std::runtime_error("Current Gun not found");

        InitializeFrameProperties();
        UpdateGunUIProperties();
        InitializeAmmoBar();

        sf::Vector2f ammoCounterPos = sf::Vector2f{frameDrawProps.Position.x - 15, frameDrawProps.Position.y - 100};
        ammoCounter = ETG::CreateGameObjectAttached<AmmoCounter>(this, ammoCounterPos);

        // Ensure all attached objects are properly initialized and named
        for (const auto& [name, sceneObj] : GameState::GetInstance().GetSceneObjs()) {
            if (sceneObj->Owner == this && sceneObj->GetObjectName() == "_NET_WM_USER_T") {
                // Fix the problematic object by setting its name properly
                sceneObj->SetObjectNameToSelfClassName();
                std::cout << "Fixed object name: " << sceneObj->GetObjectName() << std::endl;
            }
        }
    }


    void UserInterface::Update()
    {
        GameObjectBase::Update();

        // Get current gun and update properties
        if ((CurrentGun = hero->GetCurrentHoldingGun()))
        {
            UpdateGunUIProperties();

            // Update all ammo UI components with current gun
            ammoIndicators->SetGun(CurrentGun);
            ammoCounter->SetAmmo(CurrentGun->MagazineAmmo, CurrentGun->AmmoSize);
        }

        // Update UI components
        ammoBarBottom->Update(); //For now this update not doing anyhting
        ammoIndicators->Update(); // This will update top bar position through callback
        ammoBarTop->Update();
    }

    void UserInterface::Draw()
    {
        // Draw the frame
        SpriteBatch::Draw(frameDrawProps);

        // Draw the gun if available
        if (CurrentGun && CurrentGun->Texture)
        {
            SpriteBatch::Draw(gunDrawProps);
            ammoCounter->Draw();
        }

        // Draw ammo bars and indicators
        ammoBarBottom->Draw();
        ammoIndicators->Draw(); // Draw indicators between bars
        ammoBarTop->Draw();
    }

    void UserInterface::InitializeFrameProperties()
    {
        const std::string ResPath = RESOURCE_PATH;

        // Load textures
        frameTexture = std::make_shared<sf::Texture>();
        if (!frameTexture->loadFromFile(ResPath + "/UI/Frame.png"))
            throw std::runtime_error("Failed to load Frame.png");

        // Calculate game screen size (accounting for engine UI)
        GameScreenSize = {
            static_cast<float>(Globals::ScreenSize.x - GameState::GetInstance().GetEngineUISize().x),
            static_cast<float>(Globals::ScreenSize.y)
        };

        frameSize = frameTexture->getSize();
        const float FrameOffsetX = GameScreenSize.x * (FrameOffsetPerc.x / 100);
        const float FrameOffsetY = GameScreenSize.y * (FrameOffsetPerc.y / 100);
        framePosition = {
            (GameScreenSize.x - FrameOffsetX - frameSize.x / 2),
            (GameScreenSize.y - FrameOffsetY - frameSize.y / 2)
        };

        // Set up frame draw properties
        frameDrawProps.Texture = frameTexture.get();
        frameDrawProps.Position = framePosition;
        frameDrawProps.Scale = {1.0f, 1.0f};
        frameDrawProps.Origin = {
            static_cast<float>(frameSize.x) / 2.0f,
            static_cast<float>(frameSize.y) / 2.0f
        };
    }
    
    // Update the Update method

    void UserInterface::InitializeAmmoBar()
    {
        // Calculate base X position for ammo bars
        float ammoBarX = framePosition.x + (frameSize.x / 2) + (GameScreenSize.x * AmmoBarOffsetPercX / 100);

        // Create and position bottom ammo bar
        ammoBarBottom = CreateGameObjectAttached<AmmoBarUI>(this);
        ammoBarBottom->SetPosition({ammoBarX, framePosition.y + InitialAmmoBarOffsetY});
        ammoBarBottom->FlipTexture(false, true);

        // Create top ammo bar with an initial position (will be updated by indicators)
        ammoBarTop = CreateGameObjectAttached<AmmoBarUI>(this);
        ammoBarTop->SetPosition({ammoBarX, framePosition.y - InitialAmmoBarOffsetY});

        // Create ammo indicators
        ammoIndicators = CreateGameObjectAttached<AmmoIndicatorsUI>(this);
        ammoIndicators->SetGun(CurrentGun);
        ammoIndicators->SetBottomBar(ammoBarBottom.get());
        ammoBarBottom->SetPosition({ammoBarBottom->GetPosition() + sf::Vector2f{0, ammoIndicators->EachAmmoSpacing } }); //TODO: Not sure to remove this line or not

        // Set callback for updating the top bar position
        ammoIndicators->SetTopBarPositionCallback([this](float topY)
        {
            if (ammoBarTop)
            {
                ammoBarTop->SetPosition({ammoBarTop->GetPosition().x, topY});
            }
        });
    }

    void UserInterface::UpdateGunUIProperties()
    {
        CurrentGun = hero->GetCurrentHoldingGun();
        gunDrawProps.Texture = CurrentGun->Texture.get();
        gunDrawProps.Position = framePosition;
        gunDrawProps.Scale = {3.0f, 3.0f}; // UI gun scale factor
        gunDrawProps.Origin = {
            static_cast<float>(CurrentGun->Texture->getSize().x) / 2.0f,
            static_cast<float>(CurrentGun->Texture->getSize().y) / 2.0f
        };
    }
}
