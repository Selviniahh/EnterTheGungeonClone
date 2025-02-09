#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <filesystem>

//A forward declaration I've never seen before
namespace ETG {class SpriteBatch;}

namespace ETG::Globals
{
    //Elapsed time in seconds. AKA Delta Time
    extern float FrameTick;
    extern float ElapsedTimeSeconds;

    //Window and rendering resources
    extern std::shared_ptr<sf::RenderWindow> Window;
    extern sf::Font Font;
    extern sf::Vector2u ScreenSize;
    extern float DefaultScale;
    static int FPS = 170;

    //For Zooming 
    extern sf::View MainView;

    //Function to update elapsed time
    void Update();

    //Initialize global variables
    void Initialize(const std::shared_ptr<sf::RenderWindow>& window);
    
    bool DrawSinglePixelAtLoc(const sf::Vector2f& Loc, sf::Vector2i scale = {1,1}, float rotation = 0);

}

namespace ETG
{
    //Operator overloads
    std::ostream& operator<<(std::ostream& lhs, const sf::Vector2<int>& rhs);
    std::ostream& operator<<(std::ostream& lhs, const sf::IntRect& rhs);
    std::ostream operator<<(const std::ostream& lhs, const sf::Vector2<float>& rhs);
    std::ostream& operator<<(std::ostream& lhs, const sf::FloatRect& rhs);
}
