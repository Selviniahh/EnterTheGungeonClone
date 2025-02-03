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
        std::string ResPath = (std::filesystem::current_path().parent_path() / "Resources").string();
        Frame.loadFromFile(ResPath + "/UI/Frame.png");
        Gun.loadFromFile(ResPath + "/Guns/RogueSpecial_Idle.png");
        AmmoBar.loadFromFile(ResPath + "/UI/AmmoBarUI.png");
        AmmoDisplay.loadFromFile(ResPath + "/UI/AmmoDisplay.png");

        const sf::Vector2u frameSize = Frame.getSize();
        FrameOffsetX = Globals::ScreenSize.x * 0.25; //%25 of the screensize
        FrameOffsetY = Globals::ScreenSize.y * 0.25; //%25 of the screensize
        
        FramePosition = {
            static_cast<int>(Globals::ScreenSize.x / 2.f + frameSize.x / 2.f + FrameOffsetX),
            static_cast<int>(Globals::ScreenSize.y / 2.f + frameSize.y / 2.f + FrameOffsetY)
        };

        std::cout << std::unitbuf;

        GunPosition = {
            static_cast<int>(FramePosition.x + Frame.getSize().x / 2 - GunOffsetX),
            static_cast<int>(FramePosition.y + Frame.getSize().y / 2 - GunOffsetY)
        };

        // FramePosition.y + Frame.getSize().y / 2 - GunOffsetY; //idk what this doing
        AmmoBarPosition = {
            static_cast<int>(FramePosition.x + 10 + Frame.getSize().x),
            FramePosition.y - 30
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
        frame.setPosition(static_cast<float>(FramePosition.x), static_cast<float>(FramePosition.y));
        Globals::Window->draw(frame);

        // Draw the gun
        sf::Sprite gun;
        gun.setTexture(Gun);
        gun.setPosition(static_cast<float>(GunPosition.x), static_cast<float>(GunPosition.y));
        gun.setScale(3.f,3.f);
        ETG::Globals::SpriteBatch.draw(gun);
        
        // Draw ammo bar
        sf::Sprite ammo;
        ammo.setTexture(AmmoBar);
        ammo.setPosition(static_cast<float>(AmmoBarPosition.x), static_cast<float>(AmmoBarPosition.y));
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
