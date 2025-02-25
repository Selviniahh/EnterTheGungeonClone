#include "Hero.h"
#include <iostream>

#include "../Managers/GameState.h"
#include "../Managers/InputManager.h"
#include "../Managers/SpriteBatch.h"
#include "Components/HeroAnimComp.h"

float ETG::Hero::MouseAngle = 0;
ETG::Direction ETG::Hero::CurrentDirection{};
bool ETG::Hero::IsShooting{};

ETG::Hero::Hero(const sf::Vector2f Position) : HandTex({}), HandPos({})
{
    this->Position = Position;
    Depth = 2;
    GameState::GetInstance().SetHero(this);

    //Set gun
    RogueSpecial = ETG::CreateGameObject<class RogueSpecial>(HandPos);

    //Set animation
    AnimationComp = ETG::CreateGameObject<HeroAnimComp>();

    MoveComp = ETG::CreateGameObject<HeroMoveComp>();

    if (!HandTex.loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Player" / "rogue_hand_001.png").generic_string()))
        std::cerr << "Failed to load hand texture" << std::endl;
}

void ETG::Hero::Update()
{
    //override
    GameObject::Update();
    
    InputComp.Update(*this);
    MoveComp->Update();

    //Animations
    AnimationComp->Update();
    Origin = AnimationComp->AnimManagerDict[CurrentHeroState].AnimationDict[AnimationComp->CurrentAnimStateKey].Origin;
    RelativeHandLoc = AnimationComp->FlipSprites(CurrentDirection, *RogueSpecial);
    SetHandTexLoc();

    //Gun
    RogueSpecial->SetPosition(HandPos + RelativeGunOffsetPos);
    RogueSpecial->Rotation = MouseAngle;
    RogueSpecial->Update();
    if (IsShooting) RogueSpecial->Shoot();
}

void ETG::Hero::Draw()
{
    RogueSpecial->Draw();

    auto& DrawProps = GetDrawProperties();
    
    //Hero self animation draw
    AnimationComp->Draw(DrawProps.Position, DrawProps.Origin, DrawProps.Scale, DrawProps.Rotation, DrawProps.Depth);
    SpriteBatch::SimpleDraw(HandTex, HandPos);
    Globals::DrawSinglePixelAtLoc(DrawProps.Position);

    // Debug bounds drawing
    DrawHeroBounds();
}

sf::FloatRect ETG::Hero::HeroBounds() const
{
    const auto& currTexRect = AnimationComp->CurrTexRect;
    return {
        Position.x - currTexRect.width / 2.f,
        Position.y - currTexRect.height / 2.f,
        static_cast<float>(currTexRect.width),
        static_cast<float>(currTexRect.height)
    };
}

void ETG::Hero::DrawHeroBounds() const
{
    const sf::FloatRect bounds = HeroBounds();
    sf::RectangleShape boundsBox(sf::Vector2f(bounds.width, bounds.height));
    boundsBox.setPosition(bounds.left, bounds.top);
    boundsBox.setOutlineColor(sf::Color::Red);
    boundsBox.setOutlineThickness(1.f);
    boundsBox.setFillColor(sf::Color::Transparent);
    Globals::Window->draw(boundsBox);
}

void ETG::Hero::SetHandTexLoc()
{
    HandPos = Position + RelativeHandLoc;
    HandPos.x -= static_cast<float>(HandTex.getSize().x) / 2;
    HandPos.y -= static_cast<float>(HandTex.getSize().y) / 2;
}
