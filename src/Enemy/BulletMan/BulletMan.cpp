#include "BulletMan.h"
#include <filesystem>
#include <iostream>

#include "../../Core/Factory.h"
#include "../../Managers/DebugTexts.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Utils/Math.h"
#include "../../Utils/StrManipulateUtil.h"

ETG::BulletMan::BulletMan(const sf::Vector2f& position)
{
    this->Position = position;
    BulletManState = EnemyStateEnum::Idle; // Initialize state
    BulletManDir = Direction::Right; // Initialize direction
    Depth = 2; // Set depth like Hero does
}

void ETG::BulletMan::Initialize()
{
    EnemyBase::Initialize();

    // Create animation component (exactly like Hero does)
    AnimationComp = ETG::CreateGameObjectAttached<BulletManAnimComp>(this);
    AnimationComp->Initialize();
    AnimationComp->Update(); // Set the Texture during Initialization (like Hero)

    // Don't load texture directly - let the animation component handle it
    
    GameObjectBase::Initialize();
}

void ETG::BulletMan::Update()
{
    GameObjectBase::Update(); // Start with base update

    // Update direction to hero
    BulletManDir = DirectionUtils::GetDirectionToHero(Hero, Position);
    
    const auto DirectionSelfToHero = Math::Normalize(Hero->GetPosition() - Position);
    DebugTextManager::QueueText("DirectionSelfToHero " + std::to_string(DirectionSelfToHero.x) + "Y: " + std::to_string(DirectionSelfToHero.y));
    DebugTextManager::QueueText("Enemy Direction: " + std::string(EnumToString(BulletManDir)));

    // For now just use Idle state
    BulletManState = EnemyStateEnum::Idle;
    
    // Update animation (just like Hero)
    if (AnimationComp)
    {
        // Flip sprites based on direction like Hero does
        AnimationComp->FlipSpritesX(BulletManDir, *this);
        AnimationComp->Update();
        
        // Make sure we get the texture from the animation component
        if (AnimationComp->CurrentTex)
        {
            Texture = AnimationComp->CurrentTex;
            // Make sure CurrentFrame is set
        }
    }

    EnemyBase::Update(); // End with base update like Hero does
}

void ETG::BulletMan::Draw()
{
    EnemyBase::Draw();
    SpriteBatch::Draw(GetDrawProperties());
}

ETG::BulletManAnimComp::BulletManAnimComp()
{
    IsGameObjectUISpecified = false; // Set like HeroAnimComp
    SetAnimations(); // Call SetAnimations directly like HeroAnimComp
}

void ETG::BulletManAnimComp::Initialize()
{
    BaseAnimComp<EnemyStateEnum>::Initialize();
    
    // Set initial state here (after initialization)
    if (Owner)
    {
        BulletMan = dynamic_cast<class BulletMan*>(Owner);
        if (BulletMan)
        {
            CurrentState = BulletMan->BulletManState;
            CurrentAnimStateKey = DirectionUtils::GetBulletManIdleEnum(BulletMan->BulletManDir);
        }
    }
}

void ETG::BulletManAnimComp::SetAnimations()
{
    BaseAnimComp::SetAnimations();

    //Idle Animation
    const auto idleAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Enemy/BulletMan/Idle", "bullet_idle_back_001", "png", 0.15f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Idle", "bullet_idle_right_001", "png", 0.15f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Idle", "bullet_idle_left_001", "png", 0.15f),
    };

    auto IdleAnimManager = AnimationManager();
    for (int i = 0; i < idleAnims.size(); ++i)
    {
        IdleAnimManager.AddAnimation(BulletManIdleEnumValues[i], idleAnims[i]);
        IdleAnimManager.SetOrigin(BulletManIdleEnumValues[i], sf::Vector2f{(float)idleAnims[i].FrameRects[0].width / 2, (float)idleAnims[i].FrameRects[0].height / 2});
    }
    AnimManagerDict[EnemyStateEnum::Idle] = IdleAnimManager;

    //Run animation
    const auto RunAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Enemy/BulletMan/Run", "bullet_run_left_001", "png", 0.15f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Run", "bullet_run_left_back_001", "png", 0.15f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Run", "bullet_run_right_001", "png", 0.15f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Run", "bullet_run_right_back_001", "png", 0.15f),
    };

    auto RunAnimManager = AnimationManager();
    for (int i = 0; i < RunAnims.size(); ++i)
    {
        RunAnimManager.AddAnimation(BulletManRunEnumValues[i], RunAnims[i]);
        RunAnimManager.SetOrigin(BulletManRunEnumValues[i], sf::Vector2f{(float)RunAnims[i].FrameRects[0].width / 2, (float)RunAnims[i].FrameRects[0].height / 2});
    }
    AnimManagerDict[EnemyStateEnum::Run] = RunAnimManager;
}

void ETG::BulletManAnimComp::Update()
{
    // Ensure we have a valid BulletMan pointer
    if (!BulletMan)
    {
        BulletMan = dynamic_cast<class BulletMan*>(Owner);
        if (!BulletMan) return; // Safety check
    }
    
    AnimationKey newKey;

    // Set key based on state (identical to HeroAnimComp approach)
    if (BulletMan->BulletManState == EnemyStateEnum::Idle)
        newKey = DirectionUtils::GetBulletManIdleEnum(BulletMan->BulletManDir);
    
    else if (BulletMan->BulletManState == EnemyStateEnum::Run)
        newKey = DirectionUtils::GetBulletManRunEnum(BulletMan->BulletManDir);
    else
        newKey = BulletManIdleEnum::Idle_Back; // Default fallback

    // If key has changed, update and restart animation (just like HeroAnimComp)
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

    // Update base animation component with current state and key (same as HeroAnimComp)
    BaseAnimComp<EnemyStateEnum>::Update(BulletMan->BulletManState, CurrentAnimStateKey);
    
    // Make sure the texture gets transferred to the owner
    if (CurrentTex && BulletMan)
    {
        BulletMan->Texture = CurrentTex;
    }
}
