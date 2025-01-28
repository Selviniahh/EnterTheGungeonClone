#pragma once
#include <SFML/Graphics/Texture.hpp>

class Animation
{
private:
    float EachFrameSpeed;
    float AnimTimeLeft;
    int CurrentFrame = 0;
    int FrameX;
    int FrameY;
    bool Active = true;
    mutable std::vector<sf::Texture> textureCache; 

public:
    sf::IntRect CurrRect;
    sf::Texture Texture;
    sf::Vector2f Origin;
    std::vector<sf::Rect<int>> FrameRects;
    bool IsValid = true;
    float flipX = 1.0f;

    //NOTE: Rule of Five: Destructor, Copy Constructor, Copy Assignment, Move Constructor, Move Assignment
    Animation(const sf::Texture& texture, float eachFrameSpeed, int frameX, int frameY, int row = 1);
    ~Animation() = default;
    Animation(const Animation& other) = default; // Copy constructor
    Animation(Animation&& other) = default; // Move constructor
    Animation& operator=(const Animation& other) = default; // Copy assignment operator

    ///Empty constructor. The point of this is let member Animation variables default constructed.
    ///So that I can easily initialize in Initialize() function instead of directly through constructor
    Animation() = default;

    /// \brief decrement `AnimTimeLeft` If AnimTimeLeft is 0, increment CurrentFrame, restart CurrentFrame counter
    void Update();
    void Draw(sf::Vector2f position, float layerDepth) const;
    void Draw(const sf::Texture& texture, sf::Vector2f position, sf::Color color, float rotation, sf::Vector2f origin, float scale) const;

    /// Restart the animation. Set `CurrentFrame` = 0, `AnimTimeLeft` = `EachFrameSpeed`
    void Restart();
    const sf::Texture& GetCurrentFrameAsTexture() const;
    bool IsAnimationFinished() const;

    //Omit FileName's last number. If file's name is "SpriteSheet_001" Give "SpriteSheet_00"
    //There's no Y axis sprite sheet creation. Only X 
    static Animation CreateSpriteSheet(const std::string& RelativePath, const std::string& FileName, const std::string& Extension, float eachFrameSpeed);
};
