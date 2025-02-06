#pragma once;
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "../../Animation/AnimationManager.h"

class AnimationManager;

namespace ETG
{
    enum class Direction;
    
    class GunAnimComp
    {
    public:
        sf::IntRect CurrTexRect;
        sf::Vector2f RelativeOrigin{0.f, 0.f};
        sf::Texture CurrentTex;

        GunAnimComp();
        
        void SetAnimations();
        void Update(const GunStateEnum& GunState, const AnimationKey& animState);
        void Draw(sf::Vector2f position, sf::Vector2f Origin, sf::Vector2f Scale, float Rotation, float depth);
        
        std::unordered_map<GunStateEnum, AnimationManager> AnimManagerDict{};
        GunStateEnum CurrentGunState{GunStateEnum::Idle};
        AnimationKey CurrentAnimState{};
    };
}
