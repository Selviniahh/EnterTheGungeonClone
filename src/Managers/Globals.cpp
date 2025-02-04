#include "Globals.h"
#include <complex>
#include <filesystem>

namespace ETG::Globals
{
    float FrameTick = 0.0f;
    float ElapsedTimeSeconds = 0.0f;
    float DefaultScale = 1;
    std::shared_ptr<sf::RenderWindow> Window = nullptr;
    sf::Font Font;
    sf::Vector2u ScreenSize;
    sf::Clock clock;
    sf::Clock tickClock;
    sf::View MainView;
    class SpriteBatch SpriteBatch;

    void Initialize(const std::shared_ptr<sf::RenderWindow>& window)
    {
        Window = window;
        ScreenSize = window->getSize();
        SpriteBatch.begin();
        //Load font
        if (!Font.loadFromFile((std::filesystem::current_path().parent_path().parent_path() / "Resources" / "Fonts" / "SegoeUI.ttf").string()))
        {
            throw std::runtime_error("Failed to load font");
        }

        //NOTE: Set camera Location and zoom. After enemy, UI, Gun, Hero are handled, better camera and hero locations will be implemented.   
        MainView = window->getDefaultView();
        MainView.setCenter(0.f, 0.f);
        MainView.zoom(0.1f);
    }

    void Update()
    {
        //Counter starts the beginning in runtime and never stops
        ElapsedTimeSeconds = clock.getElapsedTime().asSeconds();

        //Calculate tick. In 60fps it should be: 0.016
        //It was hard for me to understand. restart sets 0 and returns total elapsed time since the last time.
        //Every loop call this will be called. So in every call it will return the total time passed in seconds.   
        const sf::Time elapsedTime = tickClock.restart();
        FrameTick = elapsedTime.asSeconds();
    }


    sf::Vector2<float> Normalize(const sf::Vector2f& vector)
    {
        const float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        if (length == 0) throw std::runtime_error("length is 0. Vector is: " + std::to_string(vector.x) + " " + std::to_string(vector.y));

        return vector / length;
    }

    bool DrawSinglePixelAtLoc(const sf::Vector2f& Loc, const sf::Vector2i scale)
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
        frame.setOrigin(0.5f, 0.5f);  // Center of 1x1 pixel
        frame.setScale(scale.x, scale.y);
        frame.setPosition(Loc); // Position it at the specified location
        
        // Draw the sprite
        SpriteBatch.draw(frame);
        return false;
    }

    std::string StringifyDirection(Direction dir)
    {
        switch (dir)
        {
        case Direction::Right: return "Right";
        case Direction::FrontHandRight: return "FrontHandRight";
        case Direction::FrontHandLeft: return "FrontHandLeft";
        case Direction::Left: return "Left";
        case Direction::BackDiagonalLeft: return "BackDiagonalLeft";
        case Direction::BackHandLeft: return "BackHandLeft";
        case Direction::BackHandRight: return "BackHandRight";
        case Direction::BackDiagonalRight: return "BackDiagonalRight";
        }
        // If there’s no match, return something
        return "Unknown Direction";
    }

    void Renderer::SimpleDraw(const sf::Texture& tex, const sf::Vector2f& pos)
    {
        sf::Sprite frame;
        frame.setTexture(tex);
        frame.setScale(Globals::DefaultScale, Globals::DefaultScale);
        frame.setPosition(pos); // Position it at the specified location

        ETG::Globals::SpriteBatch.draw(frame);
    }
}

//Operator overloads
namespace ETG
{
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
}
