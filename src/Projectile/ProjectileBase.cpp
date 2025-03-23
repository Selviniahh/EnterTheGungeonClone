#include "ProjectileBase.h"
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
    Position += ETG::Globals::FrameTick * ProjVelocity;
    GameObjectBase::Update();
}

void ETG::ProjectileBase::Draw()
{
    //Calling this is necessary. When the projectiles are spawned, Update Function called After Draw. Somehow Update needs to be called before Draw. Calling this function instead is just a temp solution. 
    // ComputeDrawProperties();
    
    auto& DrawableProps = GetDrawProperties();
    sf::Sprite frame;
    frame.setTexture(*Texture);
    frame.setOrigin(frame.getTexture()->getSize().x /2,frame.getTexture()->getSize().y /2);
    frame.setPosition(DrawableProps.Position);
    frame.setRotation(DrawableProps.Rotation);
    ETG::GlobSpriteBatch.Draw(frame,0);
}
