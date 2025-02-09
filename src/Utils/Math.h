#pragma once
#include <SFML/System/Vector2.hpp>
#include <complex>
#include <numbers>

class Math
{
    Math() = delete;

public:
    //length: Compute the magnitude of the vector using the formula sqrt(x^2 + y^2)
    // Division: Divide the vector components by the magnitude to scale it to a unit vector (length 1).
    template <typename T>
    static inline sf::Vector2<T> Normalize(const sf::Vector2<T>& Vector)
    {
        const float length = std::sqrt(Vector.x * Vector.x + Vector.y * Vector.y);
        if (length == 0) throw std::runtime_error("length is 0. Vector is: " + std::to_string(Vector.x) + " " + std::to_string(Vector.y));

        return Vector / length;
    }

    template <typename T>
    static inline T SizeSquared(const sf::Vector2<T>& Vector)
    {
        return Vector.x * Vector.x + Vector.y * Vector.y;
    }

    template <typename T>
    static inline float RadiansToDegrees(T radians)
    {
        return radians * (180.0f / std::numbers::pi);
    }

    template <typename T>
    static float Length(sf::Vector2<T> vector)
    {
        return std::sqrt(SizeSquared(vector));
    }
};
