#include "EnemyMoveCompBase.h"
#include "../../Core/GameObjectBase.h"
#include "../../Managers/GameState.h"
#include "../../Characters/Hero.h"
#include "../../Utils/Math.h"
#include "../EnemyBase.h"

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
        
        if (!OwnerEnemy || !OwnerEnemy->CanMove()) return;
        
        UpdateAIMovement();
    }

    void EnemyMoveCompBase::UpdateAIMovement()
    {
        //For now, we won't need this for a very long time. Firstly we need a map editor, then map itself and loader, all procedurally generated which shouldn't take less than 2-3 thousand lines of code
        //So what we need to worry about is when more enemies are collectively moving to hero, they will bump each other. So one enemy is an obstacle for another one.
        //After map has been created, we should just use A* to handle all pathfinding.
        //However, for now I'll just create second bigger collision component. If bigger collision component is colliding with another enemy, we will just halve enemy's velocity until collision is over.
        //It's best to do this in enemy base as default obstacle handling. When all the map is done, (probably will never) we will remove this and just use A* to handle Heuristic pathfinding.
        //For now overridden BulletManMoveComp will just move to hero all blindness. 
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
