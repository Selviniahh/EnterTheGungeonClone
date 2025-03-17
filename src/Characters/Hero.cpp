#include "Hero.h"
#include <filesystem>
#include "../Managers/GameState.h"
#include "../Managers/SpriteBatch.h"
#include "../Guns/RogueSpecial/RogueSpecial.h"
#include "../Guns/VFX/ReloadText.h"
#include "Components/HeroAnimComp.h"
#include "Components/HeroMoveComp.h"
#include "Components/InputComponent.h"
#include "Hand/Hand.h"
#include "../Guns/VFX/ReloadText.h"

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
    InputComp = ETG::CreateGameObjectAttached<InputComponent>(this);

    Hero::Initialize();
}

void ETG::Hero::Initialize()
{
    GameObjectBase::Initialize();
    ReloadText->LinkToGun(dynamic_cast<GunBase*>(RogueSpecial.get()));
}

ETG::Hero::~Hero() = default;

void ETG::Hero::Update()
{
    GameObjectBase::Update();

    InputComp->Update(*this);
    MoveComp->Update();

    // Animations - still need direct access to HeroAnimComp for specialized functionality
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

    if (IsShooting && RogueSpecial->MagazineAmmo != 0)
    {
        RogueSpecial->Shoot();
    }

    ReloadText->Update();
    GameObjectBase::Update();
}

void ETG::Hero::Draw()
{
    GameObjectBase::Draw();
    RogueSpecial->Draw();
    SpriteBatch::Draw(GetDrawProperties());

    ReloadText->Draw();
    Hand->Draw();
}

ETG::GunBase* ETG::Hero::GetCurrentHoldingGun() const
{
    return dynamic_cast<GunBase*>(RogueSpecial.get());
}
