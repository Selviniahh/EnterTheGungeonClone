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
        sf::Vector2f GunPosition;
        sf::Vector2f AmmoBarPosition;
        std::vector<sf::Vector2i> AmmoArr{};
        sf::Vector2f FramePosition;
        int lastAmmoCount = 8;
        bool IsReloaded = true;
        bool RemoveLast = true;

        float frameOffsetPercX = 4;
        float frameOffsetPercY = 3.5;
        sf::Vector2f FrameOffsetPerc{4,3.5};
        float AmmoBarOffsetPercX = 2.f;

    public:
        UserInterface();
        void Initialize();
        void Update();
        void Draw();
    };
}
