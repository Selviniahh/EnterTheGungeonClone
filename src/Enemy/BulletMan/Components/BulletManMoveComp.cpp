#include "BulletManMoveComp.h"
#include "../BulletMan.h"
#include "../../../Characters/Hero.h"
#include "../../../Utils/Math.h"
#include "../../../Managers/DebugTexts.h"
#include "../../../Utils/DirectionUtils.h"

// Constructor definition - THIS WAS MISSING
ETG::BulletManMoveComp::BulletManMoveComp() : EnemyMoveCompBase(100.0f, 10.0f, 5000.0f)
{
    // Set specific parameters for BulletMan
    DetectionRadius = 200.0f;
    StopDistance = 150.0f;
    MovementSpeed = 40.0f;
}

void ETG::BulletManMoveComp::Initialize()
{
    EnemyMoveCompBase::Initialize();

    // Get BulletMan pointer from owner
    if (Owner) BulletManPtr = dynamic_cast<BulletMan*>(Owner);
}

void ETG::BulletManMoveComp::UpdateAIMovement()
{
    if (!BulletManPtr) return;

    if (!Hero->IsValid()) return;

    // Calculate direction and distance to hero
    const sf::Vector2f directionToHero = GetDirectionToHero();
    const float distanceToHero = GetDistanceToHero();

    // Update BulletMan's direction
    BulletManPtr->EnemyDir = DirectionUtils::GetDirectionToHero(Hero, BulletManPtr->GetPosition());

    // Move if the hero is in range and not in attack range
    if (distanceToHero > StopDistance)
    {
        // Set the state to running when moving
        BulletManPtr->EnemyState = EnemyStateEnum::Run;
        MaxSpeed = MovementSpeed;

        // Update movement
        sf::Vector2f position = BulletManPtr->GetPosition();
        UpdateMovement(directionToHero, position);
        BulletManPtr->SetPosition(position);
        BulletManPtr->BulletManShoot();
    }
    else
    {
        // Either at proper distance or in cooldown, just idle
        BulletManPtr->EnemyState = EnemyStateEnum::Idle;

        // Gradually slow down
        Velocity *= 0.9f;
        BulletManPtr->BulletManShoot();
    }
}