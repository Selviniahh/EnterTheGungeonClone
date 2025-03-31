#include "BulletMan.h"
#include <filesystem>
#include "../../Core/Factory.h"
#include "../../Managers/DebugTexts.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Utils/Math.h"
#include "../../Utils/StrManipulateUtil.h"
#include "Components/BulletManAnimComp.h"
#include "Components/BulletManMoveComp.h"
#include "../../Core/Components/CollisionComponent.h"
#include "../../Characters/Hand/Hand.h"

ETG::BulletMan::BulletMan(const sf::Vector2f& position)
{
    this->Position = position;
    Depth = 2; // Set depth like Hero does

    Hand = ETG::CreateGameObjectAttached<class Hand>(this);
    
    //Collision
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 10.0f;
    CollisionComp->CollisionVisualizationColor = sf::Color::Magenta;
    CollisionComp->SetCollisionEnabled(true);

    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        if (auto* heroObj = dynamic_cast<class Hero*>(eventData.Other))
        {
            // Stop movement when colliding with hero
            isInAttackRange = true;
        }
    });

    CollisionComp->OnCollisionExit.AddListener([this](const CollisionEventData& eventData)
    {
        if (auto* heroObj = dynamic_cast<class Hero*>(eventData.Other))
        {
            // Resume movement when no longer colliding with hero
            isInAttackRange = false;
        }
    });
}

ETG::BulletMan::~BulletMan() = default;

void ETG::BulletMan::Initialize()
{
    EnemyBase::Initialize();

    // Initialize animation component
    AnimationComp = ETG::CreateGameObjectAttached<BulletManAnimComp>(this);
    AnimationComp->Initialize();
    AnimationComp->Update();

    // Initialize movement component
    MoveComp = ETG::CreateGameObjectAttached<BulletManMoveComp>(this);
    MoveComp->Initialize();

    EnemyBase::Initialize();
}

void ETG::BulletMan::Update()
{
    EnemyBase::Update(); // Start with base update

    CollisionComp->Update();

    // Update movement
    MoveComp->Update();

    // Update animation Flip sprites based on direction like Hero does
    AnimationComp->FlipSpritesX(BulletManDir, *this);
    AnimationComp->Update();

    EnemyBase::Update();
}

void ETG::BulletMan::Draw()
{
    if (!IsVisible) return;
    EnemyBase::Draw();
    SpriteBatch::Draw(GetDrawProperties());
    if (CollisionComp) CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
}
