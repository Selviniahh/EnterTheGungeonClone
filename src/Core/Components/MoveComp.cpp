#include "MoveComp.h"
#include "../../Managers/Globals.h"
#include "../../Utils/Math.h"

MoveComp::MoveComp(float maxSpeed, float acceleration, float deceleration)
    : MaxSpeed(maxSpeed),
      Acceleration(acceleration),
      Deceleration(deceleration),
      Velocity(0.f, 0.f)
{
}

void MoveComp::UpdateMovement(const sf::Vector2f& inputDir, sf::Vector2f& position)
{
    const float deltaTime = ETG::Globals::FrameTick; // delta time from your globals

    if (inputDir != sf::Vector2f(0.f, 0.f))
    {
        // Accelerate: add (normalized input * acceleration * dt)
        const sf::Vector2f normDir = Math::Normalize(inputDir);
        Velocity += normDir * Acceleration * deltaTime;

        // (Optional: apply TurningBoost if you want extra force when changing direction.)

        // Clamp speed to MaxSpeed.
        float currentSpeed = Math::Length(Velocity);
        if (currentSpeed > MaxSpeed)
        {
            Velocity = Math::Normalize(Velocity) * MaxSpeed;
        }
    }
    else
    {
        // No input: decelerate.
        float currentSpeed = Math::Length(Velocity);
        float decAmount = Deceleration * deltaTime;
        if (decAmount > currentSpeed)
        {
            Velocity = sf::Vector2f(0.f, 0.f);
        }
        else
        {
            Velocity -= Math::Normalize(Velocity) * decAmount;
        }
    }

    // Update the position by velocity.
    position += Velocity * deltaTime;
}

void MoveComp::Update()
{
    // Base update—if nothing is done here, simply call the GameObject update.
    GameObject::Update();
}
