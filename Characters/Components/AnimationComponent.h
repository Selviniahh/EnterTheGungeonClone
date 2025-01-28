#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../../Animation/AnimationManager.h"
#include "../../Managers/StateEnums.h"

namespace ETG
{
    enum class HeroStateEnum;

    class AnimationComponent
    {
    public:
        sf::IntRect CurrTexRect;
        sf::Vector2f RelativeOrigin{0.f, 0.f};
        sf::Texture CurrentTex;

        AnimationComponent();
        
        void SetAnimations();
        void Update(const HeroStateEnum& heroState, const AnimationKey& animState);
        void Draw(sf::Vector2f position);
        sf::Vector2f FlipSprites(const Direction& currentDirection);
        
    private:
        std::unordered_map<HeroStateEnum, AnimationManager> AnimManagerDict{};
        HeroStateEnum CurrentHeroState{};
        AnimationKey CurrentAnimState{};
    };
}