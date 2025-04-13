#pragma once
#include <SFML/Graphics.hpp>

#include "../../Animation/Animation.h"

namespace ETG
{
    // Interface for all animation components
    class IAnimationComponent
    {
    public:
        virtual ~IAnimationComponent() = default;
        [[nodiscard]] virtual sf::IntRect GetCurrentTextureRect() const = 0;
        [[nodiscard]] const virtual Animation* GetAnimation() const = 0;
    };
}
