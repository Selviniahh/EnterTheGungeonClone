#include "ProjectileBase.h"

#include <valarray>

#include "../Characters/Hero.h"
#include "../Core/Factory.h"
#include "../Core/Components/CollisionComponent.h"
#include "../Enemy/EnemyBase.h"
#include "../Managers/Globals.h"
#include "../Managers/SpriteBatch.h" // <-- Add this include

ETG::ProjectileBase::ProjectileBase(const sf::Texture& texture, const sf::Vector2f spawnPos, const sf::Vector2f velocity, const float range, const float rotation, const float damage, const float force)
{
    Position = spawnPos;
    ProjVelocity = velocity;
    Range = range;
    Texture = std::make_shared<sf::Texture>(texture);
    Rotation = rotation;
    Damage = damage;
    Force = force;

    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 1.0f;
    CollisionComp->CollisionVisualizationColor = sf::Color::Blue;
    CollisionComp->SetCollisionEnabled(true);

    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        // Check if we collided with a projectile
        const auto heroObj = dynamic_cast<Hero*>(this->Owner->Owner);
        const auto* enemyObj = dynamic_cast<EnemyBase*>(eventData.Other);
        
        if (heroObj && enemyObj)
        {
            std::cout << "we need to play impact animation here for " << this->Owner->Owner->GetObjectName() << " 's projectile and then destroy but nevermind " << std::endl;
            MarkForDestroy(); //we cannot destroy so easily. I need to find a way to completely restructure all raw pointers to be weak pointers.
        }
    });
}

void ETG::ProjectileBase::Initialize()
{
}

void ETG::ProjectileBase::Update()
{
    if (PendingDestroy) return;
    CollisionComp->Update();

    const sf::Vector2f movement = Globals::FrameTick * ProjVelocity;
    Position += movement;

    //Calculate distance traveled so far
    const float frameDistance = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    DistanceTraveled += frameDistance;

    //If projectile has exceeded it's range destroy
    if (DistanceTraveled >= Range)
    {
        //TODO: NOTE: URGENT MOST URGENT!!!!!!!!!!!!!!!!!!!!!!!
        //NOTE: After I implemented the collision system, destroyed objects' delegates are still being called.
        //I knew this day would come where I would have to deal dangling pointers
        //I know the best way is to convert all raw non-owner pointers to be weak pointers.
        //I already not storing any raw pointers as owner. They either shared or unique pointers.
        //Just let's convert raw pointers to be weak pointers. We will check if the pointer is valid before using it all the time all the time all the time
        //Also when object is destroyed, we need to remove all the delegates from the object
        
        MarkForDestroy();
        return;
    }

    GameObjectBase::Update();
}

void ETG::ProjectileBase::Draw()
{
    IsVisible = true;
    if (CollisionComp) CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
    auto& DrawableProps = GetDrawProperties();
    sf::Sprite frame;
    frame.setTexture(*Texture);
    frame.setOrigin(frame.getTexture()->getSize().x / 2, frame.getTexture()->getSize().y / 2);
    frame.setPosition(DrawableProps.Position);
    frame.setRotation(DrawableProps.Rotation);
    ETG::GlobSpriteBatch.Draw(frame, 0);
}
