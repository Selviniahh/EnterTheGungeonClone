#include "Globals.h"
#include <filesystem>
#include "SpriteBatch.h"

namespace ETG::Globals
{
    float FrameTick = 0.0f;
    float DeltaTime = 0.0f;
    float ElapsedTimeSeconds = 0.0f;
    float DefaultScale = 1;
    std::shared_ptr<sf::RenderWindow> Window = nullptr;
    std::unique_ptr<sf::Font> Font;
    sf::Vector2u ScreenSize;
    sf::Clock clock;
    sf::Clock tickClock;
    sf::View MainView;
    sf::Time ElapsedTimeClock;

    void Initialize(const std::shared_ptr<sf::RenderWindow>& window)
    {
        Window = window;
        ScreenSize = window->getSize();
        GlobSpriteBatch.begin();

        //Load font
        const std::filesystem::path FullPath = std::filesystem::path(RESOURCE_PATH) / "Fonts" / "SegoeUI.ttf";

        Font = std::make_unique<sf::Font>();
        if (!Font->loadFromFile(FullPath.generic_string()))
        {
            throw std::runtime_error("Failed to load font");
        }

        //NOTE: Set camera Location and zoom. After enemy, UI, Gun, Hero are handled, better camera and hero locations will be implemented.   
        MainView = window->getDefaultView();
        MainView.setCenter(0.f, 0.f);
        MainView.zoom(0.2f);
    }

    void Update()
    {
        //Counter starts the beginning in runtime and never stops
        ElapsedTimeSeconds = clock.getElapsedTime().asSeconds();

        //Calculate tick. In 60fps it should be: 0.016
        //It was hard for me to understand. restart sets 0 and returns total elapsed time since the last time.
        //Every loop call this will be called. So in every call it will return the total time passed in seconds.   
        ElapsedTimeClock = tickClock.restart();
        FrameTick = ElapsedTimeClock.asSeconds();
        DeltaTime = FrameTick;
    }

    bool DrawSinglePixelAtLoc(const sf::Vector2f& Loc, const sf::Vector2i scale, const float rotation)
    {
        static sf::Texture tex;
        static bool isLoaded = false;
        if (!isLoaded)
        {
            sf::Image greenPixel;
            greenPixel.create(1, 1, sf::Color::Green);
            if (!tex.loadFromImage(greenPixel)) return true;
            isLoaded = true;
        }

        // Set up the sprite with the 1x1 green texture
        //NOTE: Order of sprite setting should be Texture -> Origin -> Scale -> Position
        sf::Sprite frame;
        frame.setTexture(tex);
        frame.setOrigin(0.5f, 0.5f); // Center of 1x1 pixel
        frame.setScale(scale.x, scale.y);
        frame.setPosition(Loc); // Position it at the specified location
        frame.setRotation(rotation);

        // Draw the sprite
        GlobSpriteBatch.draw(frame, -1);
        return false;
    }
}

//Operator overloads
std::ostream& operator<<(std::ostream& lhs, const sf::Vector2<int>& rhs)
{
    return lhs << "X: " << rhs.x << " Y: " << rhs.y << std::endl;
}

std::ostream& operator<<(std::ostream& lhs, const sf::IntRect& rhs)
{
    return lhs << "Size: " << rhs.getSize()
        << "Height: " << rhs.height
        << " Width: " << rhs.width
        << " Top: " << rhs.top
        << " Left:" << rhs.left
        << std::endl << "Position: " << rhs.getPosition() << std::endl;
}

std::ostream& operator<<(std::ostream& lhs, const sf::FloatRect& rhs)
{
    return lhs << "Left: " << rhs.left
        << ", Top: " << rhs.top
        << ", Width: " << rhs.width
        << ", Height: " << rhs.height << " Size: " << std::endl;
}

std::ostream& operator<<(std::ostream& lhs, const sf::Vector2<float>& rhs)
{
    return lhs << "X: " << rhs.x << " Y: " << rhs.y << std::endl;
};
