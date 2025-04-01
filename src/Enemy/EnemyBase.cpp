#include "EnemyBase.h"
#include "../Guns/RogueSpecial/RogueSpecial.h"
#include "../Utils/Math.h"
#include "../Core/Components/BaseHealthComp.h"
#include "../Characters/Hand/Hand.h"
#include "../Characters/Hero.h"
#include "../Core/Components/CollisionComponent.h"
#include "../Managers/Globals.h"

namespace ETG
{
    EnemyBase::EnemyBase() : HealthComp(nullptr), Hand(nullptr), Hero(GameState::GetInstance().GetHero())
    {
    }

    EnemyBase::~EnemyBase() = default;

    void EnemyBase::Initialize()
    {
        // Implementation
    }

    void EnemyBase::Update()
    {
        // Update force effect
        UpdateForce();

        GameObjectBase::Update();
    }

    void EnemyBase::Draw()
    {
        // Implementation
        GameObjectBase::Draw();
    }

    void EnemyBase::ApplyForce(const sf::Vector2f& forceDirection, float magnitude)
    {
        // Set force parameters
        ForceDirection = forceDirection;
        ForceMagnitude = magnitude;
        ForceTimer = 0.0f;
        IsBeingForced = true;

        // Broadcast force start event
        OnForceStart.Broadcast();
    }

    void EnemyBase::UpdateForce()
    {
        if (!IsBeingForced) return;

        // Increment force timer
        ForceTimer += Globals::FrameTick;

        if (ForceTimer < ForceMaxDuration)
        {
            // Calculate current force magnitude using lerp (starts strong, gradually weakens)
            const float currentForce = Math::IntervalLerp(ForceMagnitude * ForceSpeed, 0.0f, ForceMaxDuration, ForceTimer);

            // Apply force to position
            Position += ForceDirection * currentForce * Globals::FrameTick;
        }
        else
        {
            // Force effect is done
            IsBeingForced = false;

            // Broadcast force end event
            OnForceEnd.Broadcast();
        }
    }
}
