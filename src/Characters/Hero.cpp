#include "Hero.h"
#include <iostream>
#include <filesystem>
#include "../Managers/GameState.h"
#include "../Managers/SpriteBatch.h"
#include "../Guns/RogueSpecial/RogueSpecial.h"
#include "Components/HeroAnimComp.h"
#include "Components/HeroMoveComp.h"
#include "Components/InputComponent.h"
#include "../Managers/Globals.h"
#include "../Utils/TextureUtils.h"
#include "../Utils/StrManipulateUtil.h"


float ETG::Hero::MouseAngle = 0;
ETG::Direction ETG::Hero::CurrentDirection{};
bool ETG::Hero::IsShooting{};

ETG::Hero::Hero(const sf::Vector2f Position) : HandPos({})
{
    this->Position = Position;
    Depth = 2;
    GameState::GetInstance().SetHero(this);
    
    RogueSpecial = ETG::CreateGameObjectAttached<class RogueSpecial>(this,HandPos);
    AnimationComp = ETG::CreateGameObjectAttached<HeroAnimComp>(this);
    AnimationComp->Initialize();
    MoveComp = ETG::CreateGameObjectAttached<HeroMoveComp>(this);
    InputComp = ETG::CreateGameObjectAttached<InputComponent>(this);

    HandTex = std::make_shared<sf::Texture>();
    
    if (!HandTex->loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Player" / "rogue_hand_001.png").generic_string()))
        std::cerr << "Failed to load hand texture" << std::endl;
}

ETG::Hero::~Hero() = default;

void ETG::Hero::Update()
{
    // Call the base class update
    GameObjectBase::Update();
    
    InputComp->Update(*this);
    MoveComp->Update();

    // Animations - still need direct access to HeroAnimComp for specialized functionality
    AnimationComp->Update();
    
    // We need the AnimComp directly here to access AnimManagerDict
    Origin = AnimationComp->AnimManagerDict[CurrentHeroState].AnimationDict[AnimationComp->CurrentAnimStateKey].Origin;
    
    // FlipSprites is a specialized method in HeroAnimComp
    RelativeHandLoc = AnimationComp->FlipSprites(CurrentDirection, *RogueSpecial);
    SetHandTexLoc();

    //Gun
    RogueSpecial->SetPosition(HandPos + RelativeGunOffsetPos);
    RogueSpecial->Rotation = MouseAngle;
    RogueSpecial->Update();
    if (IsShooting) RogueSpecial->Shoot();

    // Use AnimComp's current texture
    Texture = AnimationComp->CurrentTex; 
}

void ETG::Hero::Draw()
{
    GameObjectBase::Draw();
    RogueSpecial->Draw();

    auto& DrawProps = GetDrawProperties();
    
    // Hero self animation draw - using specialized Draw method from HeroAnimComp
    AnimationComp->Draw(DrawProps.Position, DrawProps.Origin, DrawProps.Scale, DrawProps.Rotation, DrawProps.Depth);
    SpriteBatch::SimpleDraw(HandTex, HandPos);
    // Debug bounds drawing - now using base class method
    // DrawBounds();
}

void ETG::Hero::SetHandTexLoc()
{
    HandPos = Position + RelativeHandLoc;
    HandPos.x -= static_cast<float>(HandTex->getSize().x) / 2;
    HandPos.y -= static_cast<float>(HandTex->getSize().y) / 2;
}
