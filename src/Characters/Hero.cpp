#include "Hero.h"
#include <filesystem>

#include "../Core/Components/CollisionComponent.h"
#include "../Enemy/EnemyBase.h"
#include "../Managers/GameState.h"
#include "../Managers/SpriteBatch.h"
#include "../Guns/RogueSpecial/RogueSpecial.h"
#include "../UI/UIObjects/ReloadText.h"
#include "Components/HeroAnimComp.h"
#include "Components/HeroMoveComp.h"
#include "Components/InputComponent.h"
#include "Hand/Hand.h"

float ETG::Hero::MouseAngle = 0;
ETG::Direction ETG::Hero::CurrentDirection{};
bool ETG::Hero::IsShooting{};

ETG::Hero::Hero(const sf::Vector2f Position)
{
    this->Position = Position;
    Depth = 2;
    GameState::GetInstance().SetHero(this);

    Hand = ETG::CreateGameObjectAttached<class Hand>(this);
    RogueSpecial = ETG::CreateGameObjectAttached<class RogueSpecial>(this, Hand->GetRelativePosition());
    ReloadText = ETG::CreateGameObjectAttached<class ReloadText>(this);
    AnimationComp = ETG::CreateGameObjectAttached<HeroAnimComp>(this);
    AnimationComp->Initialize();
    AnimationComp->Update(); //Set the Texture during Initialization
    MoveComp = ETG::CreateGameObjectAttached<HeroMoveComp>(this);
    MoveComp->Initialize();
    InputComp = ETG::CreateGameObjectAttached<InputComponent>(this);

    //Collision comp:
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 1.f;
    CollisionComp->SetCollisionEnabled(true);

    Hero::Initialize();
}

void ETG::Hero::Initialize()
{
    GameObjectBase::Initialize();
    ReloadText->LinkToGun(dynamic_cast<GunBase*>(RogueSpecial.get()));

    //Set up collision delegates. Move these to initialize after it works well. 
    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        //If the collision is with enemy, just change the color of the enemy for now 
        if (auto* enemyObj = dynamic_cast<EnemyBase*>(eventData.Other))
        {
            enemyObj->SetColor(sf::Color::Blue);
        }

        //If it's ActiveItem, assign our pointer
        if (auto* activeItem = dynamic_cast<ActiveItemBase*>(eventData.Other))
        {
            CurrActiveItem = activeItem;
        }
    });

    CollisionComp->OnCollisionExit.AddListener([this](const CollisionEventData& eventData)
    {
        //Handle collisin exit
        if (auto* enemyObj = dynamic_cast<EnemyBase*>(eventData.Other))
        {
            enemyObj->SetColor(sf::Color::White);
        }
    });
}

ETG::Hero::~Hero() = default;

void ETG::Hero::Update()
{
    GameObjectBase::Update();
    CollisionComp->Update();
    InputComp->Update(*this);
    MoveComp->Update(); //NOTE: When InputComp changes `HeroPtr->CurrentHeroState` new AnimState changes needs to be reflected in `AnimationComp` then `MoveComp` or I can move all dash to AnimationComp????  

    AnimationComp->FlipSpritesY<class RogueSpecial>(CurrentDirection, *RogueSpecial);
    AnimationComp->FlipSpritesX(CurrentDirection, *this);

    AnimationComp->Update();

    //Set hand properties
    const sf::Vector2f HandOffsetForHero = AnimationComp->IsFacingRight(CurrentDirection) ? sf::Vector2f{8.f, 5.f} : sf::Vector2f{-7.f, 5.f};
    Hand->SetPosition(Position + Hand->HandOffset + HandOffsetForHero);
    Hand->Update();

    //Gun
    RogueSpecial->SetPosition(Hand->GetPosition() + Hand->GunOffset);
    RogueSpecial->Rotation = MouseAngle;
    RogueSpecial->Update();

    //Shoot only if these conditions are met 
    if (IsShooting && RogueSpecial->MagazineAmmo != 0 && !RogueSpecial->IsReloading)
    {
        RogueSpecial->PrepareShooting();
    }

    //Try to use Active item
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && CurrActiveItem)
    {
        CurrActiveItem->RequestUsage();
    }

    //Will run only if reload needed 
    ReloadText->Update();

    //If dashing do not draw gun and hand
     Hand->IsVisible = !(AnimationComp->IsDashing); 
     RogueSpecial->IsVisible = !(AnimationComp->IsDashing); 
    GameObjectBase::Update();
}

void ETG::Hero::Draw()
{
    if (!IsVisible) return;
    GameObjectBase::Draw();
    RogueSpecial->Draw();
    SpriteBatch::Draw(GetDrawProperties());

    ReloadText->Draw();
    Hand->Draw();

    if (CollisionComp) CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
}

ETG::GunBase* ETG::Hero::GetCurrentHoldingGun() const
{
    return dynamic_cast<GunBase*>(RogueSpecial.get());
}
