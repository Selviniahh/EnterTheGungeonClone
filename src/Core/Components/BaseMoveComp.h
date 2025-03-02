#pragma once
#include <SFML/System/Vector2.hpp>
#include "../ComponentBase.h"

class BaseMoveComp : public ETG::ComponentBase
{
protected:
    // Protected constructor so only derived classes can create one.
    BaseMoveComp(float maxSpeed, float acceleration, float deceleration = 8000.f);

public:
    // Parameters:
    float MaxSpeed;       // Maximum speed (magnitude)
    float Acceleration;   // Acceleration rate when input is present
    float Deceleration;   // Deceleration rate when no input

    // Current velocity (always a vector)
    sf::Vector2f Velocity;

    // Generic update function.
    // inputDir: the desired movement direction (zero vector if none).
    // position: reference to the object's position, which will be updated.
    void UpdateMovement(const sf::Vector2f& inputDir, sf::Vector2f& position);
};
