#pragma once
#include "../ComponentBase.h"
#include "../../Animation/AnimationManager.h"
#include "../../Utils/Interface/IAnimationComponent.h"
#include <memory>


namespace ETG
{
    enum class HeroStateEnum;

    template <typename StateEnum>
    class BaseAnimComp : public ComponentBase, public IAnimationComponent
    {
    public:
        // Override the base class Initialize method to register with owner
        void Initialize() override
        {
            GameObjectBase::Initialize();

            // Register with owner if we have one
            if (Owner)
            {
                Owner->SetAnimationInterface(this);
            }
        }

        virtual void SetAnimations();
        void Update(const StateEnum& stateEnum, const AnimationKey& animKey);
        virtual void Draw(const sf::Vector2f& position);
        virtual void Draw(sf::Vector2f position, sf::Vector2f Origin, sf::Vector2f Scale, float Rotation, float depth);

        // Implement IAnimationComponent interface
        [[nodiscard]] sf::IntRect GetCurrentTextureRect() const override { return CurrTexRect; }
        [[nodiscard]] sf::Vector2f GetOrigin() const override { return RelativeOrigin; }

        sf::IntRect CurrTexRect;
        sf::Vector2f RelativeOrigin{0.f, 0.f};
        std::shared_ptr<sf::Texture> CurrentTex;

        //Animation properties
        std::unordered_map<StateEnum, AnimationManager> AnimManagerDict{};
        StateEnum CurrentState;
        AnimationKey CurrentAnimStateKey;

        BOOST_DESCRIBE_CLASS(BaseAnimComp, (ComponentBase), (RelativeOrigin, CurrentTex), (), ()) //CurrTexRect, AnimManagerDict, CurrentState, CurrentAnimStateKey
    };

    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::Update(const StateEnum& stateEnum, const AnimationKey& animKey)
    {
        CurrentState = stateEnum;
        CurrentAnimStateKey = animKey;

        auto& animManager = AnimManagerDict[CurrentState];
        animManager.Update(CurrentAnimStateKey);

        const auto& animState = animManager.AnimationDict[CurrentAnimStateKey];
        CurrTexRect = animState.CurrRect;
        CurrentTex = animState.GetCurrentFrameAsTexture();
        RelativeOrigin = animManager.AnimationDict[AnimManagerDict[CurrentState].LastKey].Origin;
    }

    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::Draw(const sf::Vector2f& position)
    {
        if (!AnimManagerDict.contains(CurrentState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");

        AnimManagerDict[CurrentState].Draw(CurrentTex, position, sf::Color::White, this->Rotation, RelativeOrigin, this->Scale, this->Depth);
    }

    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::Draw(const sf::Vector2f position, const sf::Vector2f Origin, const sf::Vector2f Scale, const float Rotation, const float depth)
    {
        if (!AnimManagerDict.contains(CurrentState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");

        AnimManagerDict[CurrentState].Draw(CurrentTex, position, sf::Color::White, Rotation, Origin, Scale, depth);
    }

    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::SetAnimations()
    {
    }
}
