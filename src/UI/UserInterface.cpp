#include "UserInterface.h"
#include <filesystem>
#include "../Characters/Hero.h"
#include "../Guns/Base/GunBase.h"
#include "../Managers/GameState.h"
#include "../Managers/Globals.h"
#include "../Managers/SpriteBatch.h"
#include "../Core/Factory.h"

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
        currentGun = hero->GetCurrentHoldingGun();
        if (!currentGun) throw std::runtime_error("Current Gun not found");

        InitializeFrameProperties();
        UpdateGunUIProperties();
        InitializeAmmoBar();
    }


    void UserInterface::Update()
    {
        GameObjectBase::Update();

        // Get current gun and update properties
        if ((currentGun = hero->GetCurrentHoldingGun()))
        {
            UpdateGunUIProperties();

            // No need to explicitly call UpdateAmmoBarTopPos() anymore
            // since AmmoIndicatorsUI will handle positioning

            // Update all ammo UI components with current gun
            ammoIndicators->SetGun(currentGun);
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
        if (currentGun && currentGun->Texture)
        {
            SpriteBatch::Draw(gunDrawProps);
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
        ammoIndicators->SetGun(currentGun);
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
        currentGun = hero->GetCurrentHoldingGun();
        gunDrawProps.Texture = currentGun->Texture.get();
        gunDrawProps.Position = framePosition;
        gunDrawProps.Scale = {3.0f, 3.0f}; // UI gun scale factor
        gunDrawProps.Origin = {
            static_cast<float>(currentGun->Texture->getSize().x) / 2.0f,
            static_cast<float>(currentGun->Texture->getSize().y) / 2.0f
        };
    }
}
