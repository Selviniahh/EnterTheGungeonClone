#pragma once 

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;

protected:
    virtual void Initialize() {}
    virtual void Update() {}
    virtual void Draw() {}

    sf::Texture Texture;
    sf::Vector2f Position{0.f, 0.f};
    sf::Vector2f Scale = {1,1};
    float Rotation{};
    sf::Vector2f Origin{0.f, 0.f};
    float depth{};

public:
    [[nodiscard]] sf::Vector2f& GetPosition() {return Position;}
    [[nodiscard]] sf::Vector2f& GetScale() {return Scale;}
    [[nodiscard]] sf::Vector2f& GetOrigin() {return Origin;}
    
};