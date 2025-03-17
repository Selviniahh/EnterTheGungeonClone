#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <filesystem>
#include "../../Core/GameObjectBase.h"
#include "../../Managers/Globals.h"

namespace ETG
{
    class AmmoCounter : public GameObjectBase
    {
    public:
        explicit AmmoCounter(sf::Vector2f position);
        ~AmmoCounter() override = default;

        void Initialize() override;
        void Draw() override;

        // Set the ammo values to display
        void SetAmmo(int currentAmmo, int maxAmmo);

    private:
        sf::Text ammoText;
        sf::Font font;
        int currentAmmo = 0;
        int maxAmmo = 0;
        sf::Vector2f screenPosition; // Position on screen

        // Update the text content
        void UpdateText();
    };

    inline AmmoCounter::AmmoCounter(sf::Vector2f position)
    {
        AmmoCounter::Initialize();
        ammoText.setPosition(position);
    }

    inline void AmmoCounter::Initialize()
    {
        GameObjectBase::Initialize();

        // Load font - make sure the path is correct for your project
        if (!font.loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Fonts" / "alagard.ttf").generic_string()))
        {
            throw std::runtime_error("Failed to load font");
        }

        // Setup text properties
        ammoText.setFont(font);
        ammoText.setCharacterSize(30);
        ammoText.setFillColor(sf::Color::White);
        ammoText.setPosition(screenPosition);
        ammoText.setString("0 / 0"); // Default text

        //Set origin the center of the enter text
        const sf::FloatRect bounds = ammoText.getLocalBounds();
        ammoText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    }

    inline void AmmoCounter::SetAmmo(int current, int max)
    {
        if (current != currentAmmo || max != maxAmmo)
        {
            currentAmmo = current;
            maxAmmo = max;
            UpdateText();
        }
    }

    inline void AmmoCounter::UpdateText()
    {
        // Format the string
        const std::string displayText = std::to_string(currentAmmo) + " / " + std::to_string(maxAmmo);
        ammoText.setString(displayText);
    }

    inline void AmmoCounter::Draw()
    {
        // Draw text directly to the window
        // Note: This doesn't use SpriteBatch since text works differently
        sf::RenderWindow* window = Globals::Window.get();
        if (window)
        {
            window->draw(ammoText);
        }
    }
}
