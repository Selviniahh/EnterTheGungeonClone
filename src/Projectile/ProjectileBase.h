#pragma once
#include "../Core/GameObject.h"

class ProjectileBase : public ETG::GameObject
{
public:
    ProjectileBase() = default;
    ProjectileBase(const sf::Texture& texture,sf::Vector2f spawnPos, sf::Vector2f projVelocity, float maxProjectileRange, float rotation);

    void Initialize() override;
    void Update() override;
    void Draw() override;

    sf::Vector2f ProjVelocity;
    float MaxProjectileRang;
};
