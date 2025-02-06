#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <filesystem>
#include "SpriteBatch.h"
#include "StateEnums.h"

class SpriteBatch;

namespace ETG::Globals
{
    class Renderer
    {
    public:
        static void SimpleDraw(const sf::Texture& tex, const sf::Vector2f& pos, float Rotation = 0, sf::Vector2f origin = {1,1}, float Scale = 1, float depth = 1);
    };

    //Elapsed time in seconds
    extern float FrameTick;
    extern float ElapsedTimeSeconds;

    //Window and rendering resources
    extern std::shared_ptr<sf::RenderWindow> Window;
    extern sf::Font Font;
    extern sf::Vector2u ScreenSize;
    extern float DefaultScale;
    static int FPS = 170;
    extern SpriteBatch SpriteBatch;

    //For Zooming 
    extern sf::View MainView;

    //Function to update elapsed time
    void Update();

    //Initialize global variables
    void Initialize(const std::shared_ptr<sf::RenderWindow>& window);

    //length: Compute the magnitude of the vector using the formula sqrt(x^2 + y^2)
    // Division: Divide the vector components by the magnitude to scale it to a unit vector (length 1).
    sf::Vector2<float> Normalize(const sf::Vector2f& vector);
    bool DrawSinglePixelAtLoc(const sf::Vector2f& Loc, sf::Vector2i scale = {1,1}, float rotation = 0);

    std::string StringifyDirection(Direction dir);
}

namespace ETG
{
    //Operator overloads
    std::ostream& operator<<(std::ostream& lhs, const sf::Vector2<int>& rhs);
    std::ostream& operator<<(std::ostream& lhs, const sf::IntRect& rhs);
    std::ostream operator<<(const std::ostream& lhs, const sf::Vector2<float>& rhs);
    std::ostream& operator<<(std::ostream& lhs, const sf::FloatRect& rhs);
}
