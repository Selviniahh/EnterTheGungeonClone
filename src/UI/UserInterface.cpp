#include "UserInterface.h"
#include <filesystem>
#include <iostream>

#include "../Managers/Globals.h"

namespace ETG
{
    UserInterface::UserInterface()
    {
    }

    void UserInterface::Initialize()
    {
        const std::string ResPath = RESOURCE_PATH;
        Frame.loadFromFile(ResPath + "/UI/Frame.png");
        Gun.loadFromFile(ResPath + "/Guns/RogueSpecial_Idle.png");
        AmmoBar.loadFromFile(ResPath + "/UI/AmmoBarUI.png");
        AmmoDisplay.loadFromFile(ResPath + "/UI/AmmoDisplay.png");

        const sf::Vector2u frameSize = Frame.getSize();

        const float FrameOffsetX = Globals::ScreenSize.x * (FrameOffsetPerc.x / 100);
        const float FrameOffsetY = Globals::ScreenSize.y * (FrameOffsetPerc.y / 100);

        FramePosition = {
            (Globals::ScreenSize.x - FrameOffsetX - frameSize.x / 2),
            (Globals::ScreenSize.y - FrameOffsetY - frameSize.y / 2)
        };

        GunPosition = {FramePosition.x, FramePosition.y};

        AmmoBarPosition = {
            (FramePosition.x + (Frame.getSize().x / 2) + (Globals::ScreenSize.x * AmmoBarOffsetPercX / 100)),
            FramePosition.y
        };
    }

    void UserInterface::Update()
    {
    }

    void UserInterface::Draw()
    {
        // Just draw the textures, no reloading!
        sf::Sprite frame;

        // Draw the frame
        frame.setTexture(Frame);
        frame.setPosition(FramePosition.x, FramePosition.y);
        frame.setOrigin(Frame.getSize().x / 2, Frame.getSize().y / 2);
        Globals::DrawSinglePixelAtLoc(frame.getPosition(), {5, 5});
        Globals::Window->draw(frame);

        // Draw the gun
        sf::Sprite gun;
        gun.setTexture(Gun);
        gun.setPosition(static_cast<float>(GunPosition.x), static_cast<float>(GunPosition.y));
        gun.setOrigin(Gun.getSize().x / 2, Gun.getSize().y / 2);
        gun.setScale(3.f, 3.f);
        Globals::DrawSinglePixelAtLoc(gun.getPosition(), {5, 5});
        ETG::Globals::SpriteBatch.draw(gun);

        // Draw ammo bar
        sf::Sprite ammo;
        ammo.setTexture(AmmoBar);
        ammo.setOrigin(AmmoBar.getSize().x / 2, AmmoBar.getSize().y / 2);
        ammo.setPosition(static_cast<float>(AmmoBarPosition.x), static_cast<float>(AmmoBarPosition.y));
        Globals::DrawSinglePixelAtLoc(ammo.getPosition(), {5, 5});
        ETG::Globals::SpriteBatch.draw(ammo);

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
