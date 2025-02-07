#include "AnimationComponent.h"
#include <iostream>

#include "../../Guns/Base/GunBase.h"

namespace ETG
{
    AnimationComponent::AnimationComponent()
    {
        SetAnimations();
    }

    void AnimationComponent::Update(const HeroStateEnum& heroState, const AnimationKey& animState)
    {
        CurrentHeroState = heroState;
        CurrentAnimState = animState;

        if (!AnimManagerDict.contains(CurrentHeroState)) return;

        auto& currAnimManager = AnimManagerDict[CurrentHeroState];
        currAnimManager.Update(CurrentAnimState);

        const auto& currAnimState = currAnimManager.AnimationDict[CurrentAnimState];
        CurrTexRect = currAnimState.CurrRect;
        CurrentTex = currAnimState.GetCurrentFrameAsTexture();
        RelativeOrigin = currAnimManager.AnimationDict[AnimManagerDict[CurrentHeroState].LastKey].Origin;
    }

    void AnimationComponent::Draw(sf::Vector2f position)
    {
        float depth = 1;
        if (!AnimManagerDict.contains(CurrentHeroState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");
            
        AnimManagerDict[CurrentHeroState].Draw(position, depth);
    }

    sf::Vector2f AnimationComponent::FlipSprites(const Direction& currentDirection, GunBase& Gun)
    {
        if (!AnimManagerDict.contains(CurrentHeroState)) return {8.f, 5.f};

        auto& currAnimManager = AnimManagerDict[CurrentHeroState];
        auto& currAnimState = currAnimManager.AnimationDict[CurrentAnimState];
        sf::Vector2f relativeHandLoc;

        if (currentDirection == Direction::Right || currentDirection == Direction::FrontHandRight ||
            currentDirection == Direction::BackDiagonalRight || currentDirection == Direction::BackHandRight)
        {
            currAnimState.flipX = 1.f;
            Gun.Scale.y = 1.f;
            relativeHandLoc = {8.f, 5.f};
        }
        else
        {
            currAnimState.flipX = -1.f;
            Gun.Scale.y = -1.f;
            relativeHandLoc = {-7.f, 5.f};
        }

        return relativeHandLoc;
    }

    void AnimationComponent::SetAnimations()
    {
        const auto runAnims = std::vector<Animation>{
            Animation::CreateSpriteSheet("Player/Run/Back", "rogue_run_back_hands_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Run/BackWard", "rogue_run_backward_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Run/Forward", "rogue_run_forward_hands_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Run/Front", "rogue_run_front_hands_001", "PNG", 0.15f),
        };

        auto animManagerRun = AnimationManager{};
        for (int i = 0; i < runAnims.size(); ++i)
        {
            animManagerRun.AddAnimation(RunEnumValues[i], runAnims[i]);
            animManagerRun.SetOrigin(RunEnumValues[i], sf::Vector2f{(float)runAnims[i].FrameRects[0].width / 2, (float)runAnims[i].FrameRects[0].height / 2});
        }
        AnimManagerDict[HeroStateEnum::Run] = animManagerRun;

        const auto idleAnims = std::vector<Animation>{
            Animation::CreateSpriteSheet("Player/Idle/Back", "rogue_idle_back_hand_left_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Idle/BackWard", "rogue_idle_backwards_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Idle/Front", "rogue_idle_front_hand_left_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Idle/Right", "rogue_idle_hands_001", "PNG", 0.15f),
        };

        auto animManagerIdle = AnimationManager{};
        for (int i = 0; i < idleAnims.size(); ++i)
        {
            animManagerIdle.AddAnimation(IdleEnumValues[i], idleAnims[i]);
            animManagerIdle.SetOrigin(IdleEnumValues[i], sf::Vector2f{(float)idleAnims[i].FrameRects[0].width / 2, (float)idleAnims[i].FrameRects[0].height / 2});
        }
        AnimManagerDict[HeroStateEnum::Idle] = animManagerIdle;

        const auto dashAnims = std::vector<Animation>{
            Animation::CreateSpriteSheet("Player/Dash/Back", "rogue_dodge_back_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Dash/BackWard", "rogue_dodge_left_back_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Dash/Front", "rogue_dodge_front_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Dash/Left", "rogue_dodge_left_001", "PNG", 0.15f),
            Animation::CreateSpriteSheet("Player/Dash/Right", "rogue_dodge_left_001", "PNG", 0.15f),
        };

        auto animManagerDash = AnimationManager{};
        for (int i = 0; i < dashAnims.size(); ++i)
        {
            animManagerDash.AddAnimation(DashEnumValues[i], dashAnims[i]);
            animManagerDash.SetOrigin(DashEnumValues[i], sf::Vector2f{(float)dashAnims[i].FrameRects[0].width / 2, (float)dashAnims[i].FrameRects[0].height / 2});
        }
        AnimManagerDict[HeroStateEnum::Dash] = animManagerDash;
    }
}
