#include "ProjectileBase.h"

#include <valarray>

#include "../Core/Factory.h"
#include "../Managers/Globals.h"
#include "../Managers/SpriteBatch.h" // <-- Add this include

ETG::ProjectileBase::ProjectileBase(const sf::Texture& texture,const sf::Vector2f spawnPos, const sf::Vector2f velocity, const float range, const float rotation, const float damage, const float force)
{
    Position = spawnPos;
    ProjVelocity = velocity;
    Range = range;
    Texture = std::make_shared<sf::Texture>(texture);
    Rotation = rotation;
    Damage = damage;
    Force = force;
}

void ETG::ProjectileBase::Initialize()
{
    
}

void ETG::ProjectileBase::Update()
{
    if (PendingDestroy) return;

    const sf::Vector2f movement = Globals::FrameTick * ProjVelocity;
    Position += movement;

    //Calculate distance traveled so far
    const float frameDistance = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    DistanceTraveled += frameDistance;

    //If projectile has exceeded it's range destroy
    if (DistanceTraveled >= Range)
    {
        MarkForDestroy();
        return;
    }
    
    GameObjectBase::Update();
}

void ETG::ProjectileBase::Draw()
{
    IsVisible = true;
    auto& DrawableProps = GetDrawProperties();
    sf::Sprite frame;
    frame.setTexture(*Texture);
    frame.setOrigin(frame.getTexture()->getSize().x /2,frame.getTexture()->getSize().y /2);
    frame.setPosition(DrawableProps.Position);
    frame.setRotation(DrawableProps.Rotation);
    ETG::GlobSpriteBatch.Draw(frame,0);
}
