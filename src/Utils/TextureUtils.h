#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <memory>

// A small free function to retrieve a 1×1 pixel texture (or any color).
namespace ETG
{
    inline std::shared_ptr<sf::Texture> GetPixelTexture()
    {
        static std::shared_ptr<sf::Texture> whiteTex = nullptr;
        if (!whiteTex)
        {
            whiteTex = std::make_shared<sf::Texture>();
            sf::Image img;
            img.create(1, 1, sf::Color::White);
            whiteTex->loadFromImage(img);
        }
        return whiteTex;
    };
}
