#pragma once
#include <SFML/System/Vector2.hpp>
#include <complex>
#include <numbers>
#include "../Managers/Globals.h"

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

    template<typename T>
    static bool IsInRange(const T& value, const T& min, const T& max)
    {
        return value >= min && value <= max;
    }

    template<typename T>
    static T SinWaveLerp(T a, T b, T interval)
    {
        //Update the timer (0 to  and back) // Update timer
        static float Timer = 0.f; 
        Timer += ETG::Globals::FrameTick / interval;
        if (Timer > 1.0f) Timer = 0.0f;

        //Generate sine wave (-1 to +1)
        float sineValue = std::sin(Timer * std::numbers::pi);

        //apply lerp
        return static_cast<T>(std::lerp(a, b, sineValue));
    }
};
