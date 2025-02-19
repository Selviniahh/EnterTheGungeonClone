#include "HeroAnimComp.h"
#include "../../Guns/Base/GunBase.h"
#include "../../Guns/RogueSpecial/RogueSpecial.h"
#include "../../Managers/GameState.h"

namespace ETG
{
    class RogueSpecial;
}

template sf::Vector2f ETG::HeroAnimComp::FlipSprites<ETG::GunBase>(const Direction&, ETG::GunBase&);
template sf::Vector2f ETG::HeroAnimComp::FlipSprites<ETG::RogueSpecial>(const Direction&, RogueSpecial&);

namespace ETG
{
    HeroAnimComp::HeroAnimComp()
    {
        HeroPtr = GameState::GetInstance().GetHero();
        HeroAnimComp::SetAnimations();
    }

    template<typename... TObjects>
    sf::Vector2f HeroAnimComp::FlipSprites(const Direction& currentDirection, TObjects&... objects)
    {
        if (!AnimManagerDict.contains(CurrentState)) return {8.f, 5.f};

        const bool facingRight =
        (currentDirection == Direction::Right || currentDirection == Direction::FrontHandRight ||
         currentDirection == Direction::BackDiagonalRight || currentDirection == Direction::BackHandRight);

        //set flipX accordingly
        HeroPtr->GetScale().x = facingRight ? 1.f : -1.f;

        //Scale factor to apply in Y
        float flipFactor = facingRight ? 1.f : -1.f;

        //lambda that applies the scale flip to an object. It'll only flip gun for now. 
        auto flipObjectScale = [flipFactor](auto& obj)
        {
            obj.GetScale().y = flipFactor;
        };

        //Apply the flip to all passed objects
        (flipObjectScale(objects), ...);

        return facingRight ? sf::Vector2f{8.f, 5.f} : sf::Vector2f{-7.f, 5.f};
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
            animManagerRun.AddAnimation(RunEnumValues[i], runAnims[i]);
            animManagerRun.SetOrigin(RunEnumValues[i], sf::Vector2f{(float)runAnims[i].FrameRects[0].width / 2, (float)runAnims[i].FrameRects[0].height / 2});
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
            animManagerIdle.AddAnimation(IdleEnumValues[i], idleAnims[i]);
            animManagerIdle.SetOrigin(IdleEnumValues[i], sf::Vector2f{(float)idleAnims[i].FrameRects[0].width / 2, (float)idleAnims[i].FrameRects[0].height / 2});
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
            animManagerDash.AddAnimation(DashEnumValues[i], dashAnims[i]);
            animManagerDash.SetOrigin(DashEnumValues[i], sf::Vector2f{(float)dashAnims[i].FrameRects[0].width / 2, (float)dashAnims[i].FrameRects[0].height / 2});
        }
        AnimManagerDict[HeroStateEnum::Dash] = animManagerDash;
    }

    void HeroAnimComp::Update()
    {
        CurrentState = HeroPtr->CurrentHeroState;
        
        if (HeroPtr->CurrentHeroState == HeroStateEnum::Dash)
            CurrentAnimStateKey = InputComponent::GetDashDirectionEnum();
    
        else if (HeroPtr->CurrentHeroState == HeroStateEnum::Run)
            CurrentAnimStateKey = DirectionUtils::GetRunEnum(HeroPtr->CurrentDirection);
    
        else if (HeroPtr->CurrentHeroState == HeroStateEnum::Idle)
            CurrentAnimStateKey = DirectionUtils::GetIdleDirectionEnum(HeroPtr->CurrentDirection);
        
        BaseAnimComp<HeroStateEnum>::Update(CurrentState,CurrentAnimStateKey);
    }

    
}
