#include "BulletMan.h"
#include <filesystem>
#include "../../Core/Factory.h"
#include "../../Managers/DebugTexts.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Utils/Math.h"
#include "../../Utils/StrManipulateUtil.h"
#include "Components/BulletManAnimComp.h"

ETG::BulletMan::BulletMan(const sf::Vector2f& position)
{
    this->Position = position;
    BulletManState = EnemyStateEnum::Idle; // Initialize state
    BulletManDir = Direction::Right; // Initialize direction
    Depth = 2; // Set depth like Hero does
}

ETG::BulletMan::~BulletMan() = default;

void ETG::BulletMan::Initialize()
{
    EnemyBase::Initialize();

    AnimationComp = ETG::CreateGameObjectAttached<BulletManAnimComp>(this);
    AnimationComp->Initialize();
    AnimationComp->Update();

    EnemyBase::Initialize();
}

void ETG::BulletMan::Update()
{
    EnemyBase::Update(); // Start with base update

    // Update direction to hero
    BulletManDir = DirectionUtils::GetDirectionToHero(Hero, Position);

    //Draw debug texts on left 
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
    }

    EnemyBase::Update();
}

void ETG::BulletMan::Draw()
{
    EnemyBase::Draw();
    SpriteBatch::Draw(GetDrawProperties());
}
