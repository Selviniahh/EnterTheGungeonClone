#include "HeroAnimComp.h"
#include "InputComponent.h"
#include "../../Guns/Base/GunBase.h"
#include "../../Guns/RogueSpecial/RogueSpecial.h"
#include "../../Managers/GameState.h"
#include "../../Characters/Hero.h"

namespace ETG
{
    class RogueSpecial;
}

namespace ETG
{
    HeroAnimComp::HeroAnimComp()
    {
        HeroPtr = GameState::GetInstance().GetHero();
        IsGameObjectUISpecified = true;
        HeroAnimComp::SetAnimations();
        CurrentState = HeroPtr->CurrentHeroState;
    }

    void HeroAnimComp::SetAnimations()
    {
        BaseAnimComp::SetAnimations();

        const auto runAnims = std::vector<Animation>{
            Animation::CreateSpriteSheet("Player/Run/Back", "rogue_run_back_hands_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Run/BackWard", "rogue_run_backward_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Run/Forward", "rogue_run_forward_hands_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Run/Front", "rogue_run_front_hands_001", "png", 0.15f),
        };

        auto animManagerRun = AnimationManager{};
        for (int i = 0; i < runAnims.size(); ++i)
        {
            animManagerRun.AddAnimation(HeroRunEnumValues[i], runAnims[i]);
            animManagerRun.SetOrigin(HeroRunEnumValues[i], sf::Vector2f{(float)runAnims[i].FrameRects[0].width / 2, (float)runAnims[i].FrameRects[0].height / 2});
        }
        AnimManagerDict[HeroStateEnum::Run] = animManagerRun;

        const auto idleAnims = std::vector<Animation>{
            Animation::CreateSpriteSheet("Player/Idle/Back", "rogue_idle_back_hand_left_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Idle/BackWard", "rogue_idle_backwards_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Idle/Front", "rogue_idle_front_hand_left_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Idle/Right", "rogue_idle_hands_001", "png", 0.15f),
        };

        auto animManagerIdle = AnimationManager{};
        for (int i = 0; i < idleAnims.size(); ++i)
        {
            animManagerIdle.AddAnimation(HeroIdleEnumValues[i], idleAnims[i]);
            animManagerIdle.SetOrigin(HeroIdleEnumValues[i], sf::Vector2f{(float)idleAnims[i].FrameRects[0].width / 2, (float)idleAnims[i].FrameRects[0].height / 2});
        }
        AnimManagerDict[HeroStateEnum::Idle] = animManagerIdle;

        const auto dashAnims = std::vector<Animation>{
            Animation::CreateSpriteSheet("Player/Dash/Back", "rogue_dodge_back_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Dash/BackWard", "rogue_dodge_left_back_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Dash/Front", "rogue_dodge_front_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Dash/Left", "rogue_dodge_left_001", "png", 0.15f),
            Animation::CreateSpriteSheet("Player/Dash/Right", "rogue_dodge_left_001", "png", 0.15f),
        };

        auto animManagerDash = AnimationManager{};
        for (int i = 0; i < dashAnims.size(); ++i)
        {
            animManagerDash.AddAnimation(HeroDashEnumValues[i], dashAnims[i]);
            animManagerDash.SetOrigin(HeroDashEnumValues[i], sf::Vector2f{(float)dashAnims[i].FrameRects[0].width / 2, (float)dashAnims[i].FrameRects[0].height / 2});
        }
        AnimManagerDict[HeroStateEnum::Dash] = animManagerDash;
    }

    void HeroAnimComp::Update()
    {
        AnimationKey newKey;

        if (HeroPtr->CurrentHeroState == HeroStateEnum::Idle)
            newKey = DirectionUtils::GetHeroIdleDirectionEnum(HeroPtr->CurrentDirection);

        else if (HeroPtr->CurrentHeroState == HeroStateEnum::Run)
            newKey = DirectionUtils::GetHeroRunEnum(HeroPtr->CurrentDirection);

        else if (HeroPtr->CurrentHeroState == HeroStateEnum::Dash)
            newKey = InputComponent::GetDashDirectionEnum();

        if (newKey != CurrentAnimStateKey)
        {
            CurrentAnimStateKey = newKey;

            //Restart the key
            auto& animManager = AnimManagerDict[CurrentState];
            if (animManager.AnimationDict.contains(CurrentAnimStateKey))
            {
                animManager.AnimationDict[CurrentAnimStateKey].Restart();
            }
        }

        BaseAnimComp<HeroStateEnum>::Update(HeroPtr->CurrentHeroState, CurrentAnimStateKey);
    }
}
