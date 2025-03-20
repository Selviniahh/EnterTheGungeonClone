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

        // Calculate game screen size (accounting for engine UI)
        GameScreenSize = {
            static_cast<float>(Globals::ScreenSize.x - GameState::GetInstance().GetEngineUISize().x),
            static_cast<float>(Globals::ScreenSize.y)
        };

        InitializeFrameProperties();
        UpdateGunUIProperties();
        InitializeAmmoBar();

        auto ammoCounterPos = sf::Vector2f{RightGunFrame->GetPosition().x - 15, RightGunFrame->GetPosition().y - 100};
        ammoCounter = ETG::CreateGameObjectAttached<AmmoCounter>(this, ammoCounterPos);
    }

    void UserInterface::Update()
    {
        GameObjectBase::Update();
        RightGunFrame->Update();

        // Get current gun and update properties
        if ((CurrentGun = hero->GetCurrentHoldingGun()))
        {
            UpdateGunUIProperties();

            // Update all ammo UI components with current gun
            ammoIndicators->SetGun(CurrentGun);
            ammoCounter->SetAmmo(CurrentGun->MagazineAmmo, CurrentGun->MaxAmmo);
        }

        // Update UI components
        ammoBarBottom->Update(); //For now this update not doing anyhting
        ammoIndicators->Update(); // This will update top bar position through callback
        ammoBarTop->Update();
    }

    void UserInterface::Draw()
    {
        // Draw the frame
        RightGunFrame->Draw();

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

        DrawEquippedPassiveItemsAtLeftUI();
    }

    void UserInterface::InitializeFrameProperties()
    {
        // Load textures
        const std::string ResPath = RESOURCE_PATH;
        RightGunFrame = CreateGameObjectAttached<FrameBar>(this, ResPath + "/UI/Frame.png");

        const float RightFrameOffsetX = GameScreenSize.x * (FrameOffsetPerc.x / 100);
        const float RightFrameOffsetY = GameScreenSize.y * (FrameOffsetPerc.y / 100);

        const sf::Vector2f RightFramePosition = {
            (GameScreenSize.x - RightFrameOffsetX - RightGunFrame->Texture->getSize().x / 2),
            (GameScreenSize.y - RightFrameOffsetY - RightGunFrame->Texture->getSize().y / 2)
        };
        RightGunFrame->SetPosition(RightFramePosition);
    }

    void UserInterface::InitializeAmmoBar()
    {
        // Calculate base X position for ammo bars
        float ammoBarX = RightGunFrame->GetPosition().x + (RightGunFrame->Texture->getSize().x / 2) + (GameScreenSize.x * AmmoBarOffsetPercX / 100);

        // Create and position bottom ammo bar
        ammoBarBottom = CreateGameObjectAttached<AmmoBarUI>(this);
        ammoBarBottom->SetPosition({ammoBarX, RightGunFrame->GetPosition().y + InitialAmmoBarOffsetY});
        ammoBarBottom->FlipTexture(false, true);

        // Create top ammo bar with an initial position (will be updated by indicators)
        ammoBarTop = CreateGameObjectAttached<AmmoBarUI>(this);
        ammoBarTop->SetPosition({ammoBarX, RightGunFrame->GetPosition().y - InitialAmmoBarOffsetY});

        // Create ammo indicators
        ammoIndicators = CreateGameObjectAttached<AmmoIndicatorsUI>(this);
        ammoIndicators->SetGun(CurrentGun);
        ammoIndicators->SetBottomBar(ammoBarBottom.get());
        ammoBarBottom->SetPosition({ammoBarBottom->GetPosition() + sf::Vector2f{0, ammoIndicators->EachAmmoSpacing}}); //TODO: Not sure to remove this line or not

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
        gunDrawProps.Position = RightGunFrame->GetPosition();
        gunDrawProps.Scale = {3.0f, 3.0f}; // UI gun scale factor
        gunDrawProps.Origin = {
            static_cast<float>(CurrentGun->Texture->getSize().x) / 2.0f,
            static_cast<float>(CurrentGun->Texture->getSize().y) / 2.0f
        };
    }

    void UserInterface::DrawEquippedPassiveItemsAtLeftUI() const
    {
        int counter = 1;
        for (const auto& items : GameState::GetInstance().GetEquippedPassiveItems())
        {
            const sf::Vector2f pos = {
                InitialLeftOffsetX + (LeftXOffsetPerItem * counter++),
                GameScreenSize.y - InitialLeftOffsetY
            };

            DrawProperties DrawProps{};
            DrawProps.Position = pos;
            DrawProps.Scale = {2, 2};
            DrawProps.Texture = items->Texture.get();
            DrawProps.Origin = {(float)DrawProps.Texture->getSize().x / 2, (float)DrawProps.Texture->getSize().y / 2};
            SpriteBatch::Draw(DrawProps);
        }
    }
}
