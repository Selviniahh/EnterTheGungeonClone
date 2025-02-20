#include "ProjectileBase.h"
#include "../Managers/Globals.h"
#include "../Managers/SpriteBatch.h" // <-- Add this include

ProjectileBase::ProjectileBase(const sf::Texture& texture,const sf::Vector2f spawnPos, const sf::Vector2f projVelocity, const float maxProjectileRange, const float rotation)
{
    FinalPosition = spawnPos;
    ProjVelocity = projVelocity;
    MaxProjectileRang = maxProjectileRange;
    Texture = texture;
    BaseRotation = rotation;
}

void ProjectileBase::Initialize()
{
}

void ProjectileBase::Update()
{
    FinalPosition += ETG::Globals::FrameTick * ProjVelocity;
}

void ProjectileBase::Draw()
{
    sf::Sprite frame;
    frame.setTexture(Texture);
    frame.setOrigin(frame.getTexture()->getSize().x /2,frame.getTexture()->getSize().y /2);
    frame.setPosition(FinalPosition);
    frame.setRotation(BaseRotation);
    ETG::GlobSpriteBatch.draw(frame,0);
}
