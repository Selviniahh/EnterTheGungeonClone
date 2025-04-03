#include "BulletManAnimComp.h"
#include "../BulletMan.h"
#include "../../../Utils/DirectionUtils.h"

ETG::BulletManAnimComp::BulletManAnimComp()
{
    IsGameObjectUISpecified = true;
    BulletMan = nullptr;
    BulletManAnimComp::SetAnimations();
}

ETG::BulletManAnimComp::~BulletManAnimComp() = default;

void ETG::BulletManAnimComp::Initialize()
{
    BaseAnimComp<EnemyStateEnum>::Initialize();
    
    // Set initial state here
    if (Owner)
    {
        BulletMan = dynamic_cast<class BulletMan*>(Owner);
        if (BulletMan)
        {
            CurrentState = BulletMan->EnemyState;
            CurrentAnimStateKey = DirectionUtils::GetBulletManIdleEnum(BulletMan->EnemyDir);
        }
    }
}

void ETG::BulletManAnimComp::SetAnimations()
{
    BaseAnimComp::SetAnimations();

    // Idle Animation
    const auto idleAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Enemy/BulletMan/Idle", "bullet_idle_back_001", "png", 0.15f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Idle", "bullet_idle_right_001", "png", 0.15f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Idle", "bullet_idle_right_001", "png", 0.15f),
    };
    AddAnimationsForState<BulletManIdleEnum>(EnemyStateEnum::Idle, idleAnims);

    // Run animation
    const auto runAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Enemy/BulletMan/Run", "bullet_run_left_001", "png", 0.12f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Run", "bullet_run_left_back_001", "png", 0.12f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Run", "bullet_run_right_001", "png", 0.12f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Run", "bullet_run_right_back_001", "png", 0.12f),
    };
    AddAnimationsForState<BulletManRunEnum>(EnemyStateEnum::Run, runAnims);
    
    // Shooting animation
    const auto shootingAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Enemy/BulletMan/Shooting", "bullet_shooting_left_001", "png", 0.1f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Shooting", "bullet_shooting_right_001", "png", 0.1f),
    };
    AddAnimationsForState<BulletManShootingEnum>(EnemyStateEnum::Shooting, shootingAnims);
    
    // Hit animation
    const auto hitAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Enemy/BulletMan/Hit", "bullet_hit_back_left_001", "png", 0.08f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Hit", "bullet_hit_back_right_001", "png", 0.08f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Hit", "bullet_hit_left_001", "png", 0.08f),
        Animation::CreateSpriteSheet("Enemy/BulletMan/Hit", "bullet_hit_right_001", "png", 0.08f),
    };
    AddAnimationsForState<BulletManHitEnum>(EnemyStateEnum::Hit, hitAnims);
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

    // Set key based on state (similar to HeroAnimComp approach)
    if (BulletMan->EnemyState == EnemyStateEnum::Idle)
        newKey = DirectionUtils::GetBulletManIdleEnum(BulletMan->EnemyDir);
    
    else if (BulletMan->EnemyState == EnemyStateEnum::Run)
        newKey = DirectionUtils::GetBulletManRunEnum(BulletMan->EnemyDir);
    
    else if (BulletMan->EnemyState == EnemyStateEnum::Shooting)
        newKey = DirectionUtils::GetBulletManShootingEnum(BulletMan->EnemyDir);
    
    else if (BulletMan->EnemyState == EnemyStateEnum::Hit)
        newKey = DirectionUtils::GetBulletManHitEnum(BulletMan->EnemyDir);
    
    else
        newKey = BulletManIdleEnum::Idle_Back; // Default fallback

    // Update base animation component with current state and key
    BaseAnimComp<EnemyStateEnum>::Update(BulletMan->EnemyState, newKey);
}