#include "Hero.h"
#include <filesystem>
#include "../Managers/GameState.h"
#include "../Managers/SpriteBatch.h"
#include "../Guns/RogueSpecial/RogueSpecial.h"
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
}

ETG::Hero::~Hero() = default;

void ETG::Hero::Update()
{
    GameObjectBase::Update();

    InputComp->Update(*this);
    MoveComp->Update();

    // Animations - still need direct access to HeroAnimComp for specialized functionality
    AnimationComp->Update();

    //NOTE: No more manually setting Origin in tick
    // Origin = AnimationComp->AnimManagerDict[CurrentHeroState].AnimationDict[AnimationComp->CurrentAnimStateKey].Origin;

    // FlipSprites is a specialized method in HeroAnimComp
    //Set hand properties
    Hand->SetPosition(Position + Hand->HandOffset + AnimationComp->FlipSprites(CurrentDirection, *RogueSpecial));
    Hand->Update();
    
    //Gun
    RogueSpecial->SetPosition(Hand->GetPosition() + Hand->GunOffset);
    RogueSpecial->Rotation = MouseAngle;
    RogueSpecial->Update();
    if (IsShooting) RogueSpecial->Shoot();
    
    //Necessary to call end of update because the texture is created at here. 
    GameObjectBase::Update();
}

void ETG::Hero::Draw()
{
    GameObjectBase::Draw();
    RogueSpecial->Draw();
    
    //NOTE: Texting new Draw function. 
    // AnimationComp->Draw(DrawProps.Position, DrawProps.Origin, DrawProps.Scale, DrawProps.Rotation, DrawProps.Depth);
    SpriteBatch::Draw(GetDrawProperties());
    
    Hand->Draw();
}

ETG::GunBase* ETG::Hero::GetCurrentHoldingGun() const
{
    return dynamic_cast<GunBase*>(RogueSpecial.get());
}
