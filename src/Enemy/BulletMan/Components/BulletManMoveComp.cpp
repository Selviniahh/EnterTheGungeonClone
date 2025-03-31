#include "BulletManMoveComp.h"
#include "../BulletMan.h"
#include "../../../Managers/GameState.h"
#include "../../../Characters/Hero.h"
#include "../../../Utils/Math.h"
#include "../../../Managers/DebugTexts.h"
#include "../../../Utils/StrManipulateUtil.h"

namespace ETG
{
    BulletManMoveComp::BulletManMoveComp() : BaseMoveComp(100.0f, 5000.0f, 5000.0f)
    {
        IsGameObjectUISpecified = true;
    }

    void BulletManMoveComp::Initialize()
    {
        BaseMoveComp::Initialize();

        // Get BulletMan pointer from owner
        if (Owner)
        {
            BulletManPtr = dynamic_cast<BulletMan*>(Owner);
        }
    }

    void BulletManMoveComp::Update()
    {
        BaseMoveComp::Update();

        if (!BulletManPtr) return;

        UpdateMovement();
    }

    bool BulletManMoveComp::IsHeroInRange() const
    {
        if (!BulletManPtr) return false;

        Hero* hero = GameState::GetInstance().GetHero();
        if (!hero) return false;

        // Calculate distance to hero
        const float distanceToHero = Math::Length(hero->GetPosition() - BulletManPtr->GetPosition());
        return distanceToHero <= DetectionRadius;
    }

    void BulletManMoveComp::UpdateMovement()
    {
        if (!BulletManPtr) return;

        const Hero* hero = GameState::GetInstance().GetHero();
        if (!hero) return;

        // Calculate direction and distance to hero
        const sf::Vector2f directionToHero = Math::Normalize(hero->GetPosition() - BulletManPtr->GetPosition());
        const float distanceToHero = Math::Length(hero->GetPosition() - BulletManPtr->GetPosition());

        // Debug information
        DebugTextManager::QueueText("BulletMan Distance: " + std::to_string(distanceToHero));
        DebugTextManager::QueueText("BulletMan Direction: " + std::to_string(directionToHero.x) + ", " + std::to_string(directionToHero.y));
        DebugTextManager::QueueText("BulletMan Speed: " + std::to_string(MovementSpeed));

        // Update BulletMan's direction (used for animations)
        BulletManPtr->BulletManDir = DirectionUtils::GetDirectionToHero(hero, BulletManPtr->GetPosition());

        // Move if the hero is in range and not in attack range
        if (distanceToHero > StopDistance && !BulletManPtr->isInAttackRange)
        {
            // Set the state to running when moving
            BulletManPtr->BulletManState = EnemyStateEnum::Run;

            MaxSpeed = MovementSpeed;

            //Update movement at directionToHero
            sf::Vector2f position = BulletManPtr->GetPosition();
            BaseMoveComp::UpdateMovement(directionToHero, position);
            BulletManPtr->SetPosition(position);
        }

        //Shoot if timer is up and attackDistance is reached
        else if (distanceToHero <= BulletManPtr->attackDistance && BulletManPtr->attackCooldownTimer <= 0)
        {
            // In attack range and cooldown finished, enter shooting state
            BulletManPtr->BulletManState = EnemyStateEnum::Shooting;
            BulletManPtr->attackCooldownTimer = BulletManPtr->attackCooldown; // Reset cooldown

            // Stop movement
            Velocity = {0.0f, 0.0f};
        }
        else
        {
            // Either at proper distance or in cooldown, just idle
            BulletManPtr->BulletManState = EnemyStateEnum::Idle;

            // Gradually slow down
            Velocity *= 0.9f;
        }

        // Update attack cooldown timer
        if (BulletManPtr->attackCooldownTimer > 0)
        {
            BulletManPtr->attackCooldownTimer -= Globals::FrameTick;
        }
    }
}
