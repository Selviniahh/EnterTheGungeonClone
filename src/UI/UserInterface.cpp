#include "UserInterface.h"
#include <filesystem>

#include "../Engine/UI/EngineUI.h"
#include "../Managers/GameState.h"
#include "../Managers/Globals.h"
#include "../Managers/SpriteBatch.h"

namespace ETG
{
    UserInterface::UserInterface()
    {
    }

    void UserInterface::Initialize()
    {
        SetObjectName("UserInterface");
        const std::string ResPath = RESOURCE_PATH;
        Frame.loadFromFile(ResPath + "/UI/Frame.png");
        Gun.loadFromFile(ResPath + "/Guns/RogueSpecial/RogueSpecial_Idle.png");
        AmmoBar.loadFromFile(ResPath + "/UI/AmmoBarUI.png");
        AmmoDisplay.loadFromFile(ResPath + "/UI/AmmoDisplay.png");

        //NOTE: GameScreenSize is subtracting GameScreenSize with the area that designated as Engine UI. So that Gun UI and Engine UI wouldn't overlap.
        GameScreenSize = {(float)Globals::ScreenSize.x - GameState::GetInstance().GetEngineUISize().x,(float)Globals::ScreenSize.y};

        const sf::Vector2u frameSize = Frame.getSize();

        const float FrameOffsetX = GameScreenSize.x * (FrameOffsetPerc.x / 100);
        const float FrameOffsetY = GameScreenSize.y * (FrameOffsetPerc.y / 100);

        Position = {
            (GameScreenSize.x - FrameOffsetX - frameSize.x / 2),
            (GameScreenSize.y - FrameOffsetY - frameSize.y / 2)
        };

        GunPosition = {Position.x, Position.y};

        AmmoBarPosition = {
            (Position.x + (Frame.getSize().x / 2) + (GameScreenSize.x * AmmoBarOffsetPercX / 100)),
            Position.y
        };
    }

    void UserInterface::Update()
    {
        GameObject::Update();
    }

    void UserInterface::Draw()
    {
        // Just draw the textures, no reloading!
        auto& DrawProps = GetDrawProperties();
        sf::Sprite frame;

        // Draw the frame
        frame.setTexture(Frame);
        frame.setPosition(DrawProps.Position.x, DrawProps.Position.y);
        frame.setOrigin(Frame.getSize().x / 2, Frame.getSize().y / 2);
        frame.setRotation(DrawProps.Rotation);
        frame.setScale(DrawProps.Scale);
        Globals::DrawSinglePixelAtLoc(frame.getPosition(), {5, 5});
        Globals::Window->draw(frame);

        // Draw the gun
        sf::Sprite gun;
        gun.setTexture(Gun);
        gun.setPosition(static_cast<float>(GunPosition.x), static_cast<float>(GunPosition.y));
        gun.setOrigin(Gun.getSize().x / 2, Gun.getSize().y / 2);
        gun.setScale(3.f, 3.f);
        Globals::DrawSinglePixelAtLoc(gun.getPosition(), {5, 5});
        GlobSpriteBatch.draw(gun,0);

        // Draw ammo bar
        sf::Sprite ammo;
        ammo.setTexture(AmmoBar);
        ammo.setOrigin(AmmoBar.getSize().x / 2, AmmoBar.getSize().y / 2);
        ammo.setPosition(static_cast<float>(AmmoBarPosition.x), static_cast<float>(AmmoBarPosition.y));
        Globals::DrawSinglePixelAtLoc(ammo.getPosition(), {5, 5});
        GlobSpriteBatch.draw(ammo, 0);

        // // Draw ammo display. Because gun not implemented yet, let it stay like this
        // sf::Sprite ammoDisplaySprite;
        // ammoDisplaySprite.setTexture(AmmoDisplay);
        // for (auto& ammoPosition : AmmoArr)
        // {
        //     ammoDisplaySprite.setPosition(static_cast<float>(ammoPosition.x), static_cast<float>(ammoPosition.y));
        //     Globals::Window->draw(ammoDisplaySprite);
        // }
    }
}
