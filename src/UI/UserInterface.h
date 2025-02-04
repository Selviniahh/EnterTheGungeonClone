#pragma once
#include <SFML/Graphics/Texture.hpp>

namespace ETG
{
    class UserInterface
    {
    private:
        sf::Texture Frame;
        sf::Texture AmmoBar;
        sf::Texture AmmoDisplay;
        sf::Texture Gun;
        sf::Vector2i GunPosition;
        sf::Vector2i AmmoBarPosition;
        std::vector<sf::Vector2i> AmmoArr{};
        sf::Vector2i FramePosition;
        int lastAmmoCount = 8;
        bool IsReloaded = true;
        bool RemoveLast = true;

        float frameOffsetPercX = 30;
        float frameOffsetPercY = 30;
        int AmmoBarOffsetX = 30;

    public:
        UserInterface();
        void Initialize();
        void Update();
        void Draw();
    };
}
