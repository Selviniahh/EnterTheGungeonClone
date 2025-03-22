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
        UpdateGunUIProperties();
        
        RightGunFrame->Update();
        LeftActiveItemFrame->Update();

        // Get current gun and update properties
        if ((CurrentGun = hero->GetCurrentHoldingGun()))
        {
            RightGunFrame->SetGun(CurrentGun);

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
        LeftActiveItemFrame->Draw();

        // Draw ammo bars and indicators
        ammoBarBottom->Draw();
        ammoIndicators->Draw(); // Draw indicators between bars
        ammoBarTop->Draw();
        ammoCounter->Draw();

        DrawEquippedPassiveItemsAtLeftUI();
    }

    void UserInterface::InitializeFrameProperties()
    {
        // Load textures
        const std::string ResPath = RESOURCE_PATH;

        //NOTE: Right gun frame
        // Create right frame for gun display
        RightGunFrame = CreateGameObjectAttached<FrameBar>(this, ResPath + "/UI/Frame.png", BarType::GunBar);

        const float RightFrameOffsetX = GameScreenSize.x * (RightFrameOffsetPerc.x / 100);
        const float RightFrameOffsetY = GameScreenSize.y * (RightFrameOffsetPerc.y / 100);

        const sf::Vector2f RightFramePosition = {
            (GameScreenSize.x - RightFrameOffsetX - RightGunFrame->Texture->getSize().x / 2),
            (GameScreenSize.y - RightFrameOffsetY - RightGunFrame->Texture->getSize().y / 2)
        };
        RightGunFrame->SetPosition(RightFramePosition);

        // Set the current gun to the frame
        RightGunFrame->SetGun(CurrentGun);

        //NOTE: Left item frame 
        // Create left frame for active item display (positioned on left side)
        LeftActiveItemFrame = CreateGameObjectAttached<FrameBar>(this, ResPath + "/UI/Frame.png", BarType::ActiveItemBar);

        const float LeftFrameOffsetX = GameScreenSize.x * (LeftFrameOffsetPerc.x / 100);
        const float LeftFrameOffsetY = GameScreenSize.y * (LeftFrameOffsetPerc.y / 100);
        
        const sf::Vector2f LeftFramePosition = {
            (LeftFrameOffsetX + LeftActiveItemFrame->Texture->getSize().x / 2),
            (GameScreenSize.y - LeftFrameOffsetY - LeftActiveItemFrame->Texture->getSize().y / 2)
        };
        LeftActiveItemFrame->SetPosition(LeftFramePosition);
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
        RightGunFrame->SetGun(CurrentGun);
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
            DrawProps.Scale = {1.5, 1.5};
            DrawProps.Texture = items->Texture.get();
            DrawProps.Origin = {(float)DrawProps.Texture->getSize().x / 2, (float)DrawProps.Texture->getSize().y / 2};
            SpriteBatch::Draw(DrawProps);
        }
    }
}
