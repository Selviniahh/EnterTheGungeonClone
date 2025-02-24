#pragma once 
#include "../GameObject.h"
#include "../../Animation/AnimationManager.h"

namespace ETG
{
    enum class HeroStateEnum;

    template<typename Derived, typename StateEnum>
    class BaseAnimComp : public GameObject<Derived>
    {
    public:
        BaseAnimComp() = default;
        virtual void SetAnimations();
        void Update(const StateEnum& stateEnum, const AnimationKey& animKey);
        virtual void Draw(const sf::Vector2f& position);
        virtual void Draw(sf::Vector2f position, sf::Vector2f Origin, sf::Vector2f Scale, float Rotation, float depth);
        
        sf::IntRect CurrTexRect;
        sf::Vector2f RelativeOrigin{0.f, 0.f};
        sf::Texture CurrentTex;

        //Animation properties
        std::unordered_map<StateEnum, AnimationManager> AnimManagerDict{};
        StateEnum CurrentState;
        AnimationKey CurrentAnimStateKey;
    };

    template <typename DerivedName, typename StateEnum>
    void BaseAnimComp<DerivedName, StateEnum>::Update(const StateEnum& stateEnum, const AnimationKey& animKey)
    {
        CurrentState = stateEnum;
        CurrentAnimStateKey = animKey;

        // if (!AnimManagerDict.contains(CurrentState)) throw std::runtime_error("The state: " + CurrentState + " Is not found.");

        auto& animManager = AnimManagerDict[CurrentState];
        animManager.Update(CurrentAnimStateKey);

        const auto& animState = animManager.AnimationDict[CurrentAnimStateKey];
        CurrTexRect = animState.CurrRect;
        CurrentTex = animState.GetCurrentFrameAsTexture();
        RelativeOrigin = animManager.AnimationDict[AnimManagerDict[CurrentState].LastKey].Origin;
    }

    template <typename Derived, typename StateEnum>
    void BaseAnimComp<Derived, StateEnum>::Draw(const sf::Vector2f& position)
    {
        if (!AnimManagerDict.contains(CurrentState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");

        AnimManagerDict[CurrentState].Draw(CurrentTex, position, sf::Color::White, this->Rotation, RelativeOrigin, this->Scale, this->Depth);
    }

    template <typename Derived, typename StateEnum>
    void BaseAnimComp<Derived, StateEnum>::Draw(const sf::Vector2f position, const sf::Vector2f Origin, const sf::Vector2f Scale, const float Rotation, const float depth)
    {
        if (!AnimManagerDict.contains(CurrentState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");

        AnimManagerDict[CurrentState].Draw(CurrentTex, position, sf::Color::White, Rotation, Origin, Scale, depth);
    }

    template <typename Derived, typename StateEnum>
    void BaseAnimComp<Derived, StateEnum>::SetAnimations()
    {
    }
}
