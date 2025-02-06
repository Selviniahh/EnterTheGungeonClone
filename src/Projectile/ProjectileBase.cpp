#include "ProjectileBase.h"

#include "../Managers/Globals.h"

ProjectileBase::ProjectileBase(const sf::Texture& texture,const sf::Vector2f spawnPos, const sf::Vector2f projVelocity, const float maxProjectileRange, const float rotation)
{
    Position = spawnPos;
    ProjVelocity = projVelocity;
    MaxProjectileRang = maxProjectileRange;
    Texture = texture;
    Rotation = rotation;
}

void ProjectileBase::Initialize()
{
}

void ProjectileBase::Update()
{
    Position += ETG::Globals::FrameTick * ProjVelocity;
}

void ProjectileBase::Draw()
{
    sf::Sprite frame;
    frame.setTexture(Texture);
    frame.setOrigin(frame.getTexture()->getSize().x /2,frame.getTexture()->getSize().y /2);
    frame.setPosition(Position);
    frame.setRotation(Rotation);
    ETG::Globals::SpriteBatch.draw(frame,0);
}
