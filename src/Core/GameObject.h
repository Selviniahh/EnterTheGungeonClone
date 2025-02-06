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

    // void SetPosition(const sf::Vector2f& pos) {position = pos;}
    // const sf::Vector2f& GetPosition() const {return  position;}
    //
    // void SetOrigin(const sf::Vector2f& origin) {this->origin = origin;}
    // const sf::Vector2f& GetOrigin() const {return origin;}

    sf::Texture Texture;
    sf::Vector2f Position{0.f, 0.f};
    sf::Vector2f Origin{0.f, 0.f};
    sf::Vector2f Scale = {1,1};
};