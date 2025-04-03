#include "EnemyMoveCompBase.h"
#include "../../Core/GameObjectBase.h"
#include "../../Managers/GameState.h"
#include "../../Characters/Hero.h"
#include "../../Utils/Math.h"
#include "../EnemyBase.h"
#include "../../Utils/DirectionUtils.h"

namespace ETG
{
    EnemyMoveCompBase::EnemyMoveCompBase(const float maxSpeed, const float acceleration, const float deceleration) : BaseMoveComp(maxSpeed, acceleration, deceleration)
    {
        IsGameObjectUISpecified = true;
        Hero = GameState::GetInstance().GetHero();
    }

    EnemyMoveCompBase::~EnemyMoveCompBase() = default;

    void EnemyMoveCompBase::Initialize()
    {
        BaseMoveComp::Initialize();

        // Get EnemyBase pointer from owner
        if (Owner)
        {
            OwnerEnemy = dynamic_cast<EnemyBase*>(Owner);
        }
    }

    void EnemyMoveCompBase::Update()
    {
        BaseMoveComp::Update(); // This will handle force updates

        if (!OwnerEnemy || IsBeingForced) return;

        UpdateAIMovement();
    }

    void EnemyMoveCompBase::UpdateAIMovement()
    {
        //For now, we won't need this for a very long time. Firstly we need a map editor, then map itself and loader, all procedurally generated which shouldn't take less than 2-3 thousand lines of code
        //So what we need to worry about is when more enemies are collectively moving to hero, they will bump each other. So one enemy is an obstacle for another one.
        //After map has been created, we should just use A* to handle all pathfinding.
        //However, for now I'll just create second bigger collision component. If bigger collision component is colliding with another enemy, we will just halve enemy's velocity until collision is over.
        //It's best to do this in enemy base as default obstacle handling. When all the map is done, (probably will never) we will remove this and just use A* to handle Heuristic pathfinding.
        //For now BulletManMoveComp will just move to hero all blindness.

        if (!OwnerEnemy) return;

        if (!Hero->IsValid()) return;

        // Calculate direction and distance to hero
        const sf::Vector2f directionToHero = GetDirectionToHero();
        const float distanceToHero = GetDistanceToHero();

        // Update BulletMan's direction
        OwnerEnemy->EnemyDir = DirectionUtils::GetDirectionToHero(Hero, OwnerEnemy->GetPosition());

        // Move if the hero is in range and not in attack range
        if (distanceToHero > StopDistance)
        {
            // Set the state to running when moving
            OwnerEnemy->EnemyState = EnemyStateEnum::Run;

            // Update movement
            sf::Vector2f position = OwnerEnemy->GetPosition();
            UpdateMovement(directionToHero, position);
            OwnerEnemy->SetPosition(position);
            OwnerEnemy->EnemyShoot();
        }
        else
        {
            // Either at proper distance or in cooldown, just idle
            OwnerEnemy->EnemyState = EnemyStateEnum::Idle;

            // Gradually slow down
            Velocity *= 0.9f;
            OwnerEnemy->EnemyShoot();
        }
    }

    sf::Vector2f EnemyMoveCompBase::GetDirectionToHero() const
    {
        if (!Hero->IsValid() || !OwnerEnemy) return {0, 0};

        return Math::Normalize(Hero->GetPosition() - OwnerEnemy->GetPosition());
    }

    float EnemyMoveCompBase::GetDistanceToHero() const
    {
        if (!Hero || !OwnerEnemy) return 0;

        return Math::Length(Hero->GetPosition() - OwnerEnemy->GetPosition());
    }
}
