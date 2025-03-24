#pragma once
#include <SFML/System/Vector2.hpp>
#include <complex>
#include <numbers>
#include "../Managers/Globals.h"
#include <iostream>

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
    static inline float RadiansToDegrees(T radians)
    {
        return radians * (180.0f / std::numbers::pi);
    }
    
    static float AngleToRadian(const float angle)
    {
        return (angle * std::numbers::pi) / 180.f;
    }

    static sf::Vector2f RadianToDirection(const float rad)
    {
        return {std::cos(rad), std::sin(rad)};
    }

    template <typename T>
    static inline T VectorSizeSquared(const sf::Vector2<T>& Vector)
    {
        return Vector.x * Vector.x + Vector.y * Vector.y;
    }

    template <typename T>
    static float VectorLength(sf::Vector2<T> vector)
    {
        return std::sqrt(VectorSizeSquared(vector));
    }

    template <typename T>
    static bool IsInRange(const T& value, const T& min, const T& max)
    {
        return value >= min && value <= max;
    }

    //Used for non internally incremented timer
    //look at ReloadSlider
    template <typename T>
    static T SinWaveLerp(T a, T b, T interval, float& timer)
    {
        //Update the timer (0 to  and back)
        timer += ETG::Globals::FrameTick / interval;
        if (timer > 1.0f) timer = 0.0f;

        //Multiplying by π transforms this range into [0, π]
        //When timer = 0.5: sin(π/2) = 1 → fully at position b
        //When timer = 1.0: sin(π) = 0 → back to midpoint
        float sineValue = std::sin(timer * std::numbers::pi); 

        //apply lerp
        return static_cast<T>(std::lerp(a, b, sineValue));
    }

    //Used for internally incremented timer
    //Normally Timer needs 1 second to reach from 0 -> 1
    //If interval is 10. reaching 0 -> 1 will take 10 seconds
    //Look at FrameLeftProgressBar
    template <typename T>
    static T IntervalLerp(const T& a, const T& b, const T& interval, float timer)
    {
        std::cout << "Original Time: " << timer;
        timer /= interval;
        if (timer > 1.0f) timer = 0.0f;

        //Only provide 0 -> 1 range
        float sineValue = std::sin(timer * 0.5) + 0.5f; //NOTE: Just for printing purposes probably I wull delete this later on anyway I just don't wanna miss how I made sin 0 -> 1
        std::cout << " Timer: " << timer << " Sin: " << sineValue << std::endl;

        //apply lerp
        return static_cast<T>(std::lerp(a, b, timer));
    }

    template<typename T>
    static T ProgressLerp(T a, T b, const float progress)
    {
        //clamp progress to 0-1 range
        float t = std::max(0.0f, std::min(1.0f, progress));
        return static_cast<T>(std::lerp(a,b,t));
    }

    [[nodiscard]] static sf::Vector2f RotateVector(const float rotation, const sf::Vector2f scale, const sf::Vector2f& offset)
    {
        const float angleRad = rotation * (std::numbers::pi / 180.f);
        sf::Vector2f scaledOffset(offset.x * scale.x, offset.y * scale.y);

        return {
            scaledOffset.x * std::cos(angleRad) - scaledOffset.y * std::sin(angleRad),
            scaledOffset.x * std::sin(angleRad) + scaledOffset.y * std::cos(angleRad)
        };
    }

    struct FourCorner
    {
        sf::Vector2f TopLeft{};
        sf::Vector2f TopRight{};
        sf::Vector2f BottomLeft{};
        sf::Vector2f BottomRight{};
    };

    [[nodiscard]] static FourCorner CalculateFourCorner(sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& origin, const sf::Vector2f& scale = {1.f, 1.f})
    {
        FourCorner Corners;

        //Calculate the scaled size firstly
        const sf::Vector2f scaledSize = {size.x * scale.x, size.y * scale.y};

        //Calculate corners using the scaled size
        Corners.TopLeft = {pos.x, pos.y}; // Top-left
        Corners.TopRight = {pos.x + scaledSize.x, pos.y}; // Top-right
        Corners.BottomLeft = {pos.x, pos.y + scaledSize.y}; // Bottom-left
        Corners.BottomRight = {pos.x + scaledSize.x, pos.y + scaledSize.y}; // Bottom-right

        //Origin also always affected by scale
        sf::Vector2f scaledOrigin = {origin.x * scale.x, origin.y * scale.y};
        Corners.TopLeft -= scaledOrigin;
        Corners.TopRight -= scaledOrigin;
        Corners.BottomLeft -= scaledOrigin;
        Corners.BottomRight -= scaledOrigin;
        return Corners;
    }
    ;
};
