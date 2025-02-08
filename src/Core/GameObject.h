#pragma once 

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void Initialize() {}
    virtual void Update() {}
    virtual void Draw() {}

    sf::Texture Texture;
    sf::Vector2f Position{0.f, 0.f};
    sf::Vector2f Origin{0.f, 0.f};
    sf::Vector2f Scale = {1,1};
};