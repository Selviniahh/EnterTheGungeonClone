#include "Hero.h"
#include <iostream>
#include "../Managers/GameState.h"
#include "../Managers/InputManager.h"
#include "../Managers/SpriteBatch.h"

float ETG::Hero::MouseAngle = 0;
ETG::Direction ETG::Hero::CurrentDirection{};
bool ETG::Hero::IsShooting{};

ETG::Hero::Hero(const sf::Vector2f Position) : GameObject(), HandTex({}), HandPos({})
{
    this->Position = Position;
    GameState::GetInstance().SetHero(this);
    RogueSpecial = std::make_unique<class RogueSpecial>(HandPos);
    if (!HandTex.loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Player" / "rogue_hand_001.PNG").string()))
        std::cerr << "Failed to load hand texture" << std::endl;
}

void ETG::Hero::Update()
{
    //Hero self animation handling
    AnimationKey animState = IdleEnum::Idle_Back;
    
    InputComp.Update(*this);
    MoveComp.Update();
    
    if (CurrentHeroState == HeroStateEnum::Dash)
        animState = InputComponent::GetDashDirectionEnum();
    
    else if (CurrentHeroState == HeroStateEnum::Run)
        animState = DirectionUtils::GetRunEnum(CurrentDirection);
    
    else if (CurrentHeroState == HeroStateEnum::Idle)
        animState = DirectionUtils::GetIdleDirectionEnum(CurrentDirection);

    AnimationComp.Update(CurrentHeroState, animState);
    RelativeHandLoc = AnimationComp.FlipSprites(CurrentDirection, *RogueSpecial);
    SetHandTexLoc();

    //Gun
    RogueSpecial->GetPosition() = HandPos + sf::Vector2f{2,2};
    RogueSpecial->Rotation = MouseAngle;
    RogueSpecial->Update();
    if (IsShooting) RogueSpecial->Shoot();
}

void ETG::Hero::Draw()
{
    RogueSpecial->Draw();
    
    //Hero self animation draw
    AnimationComp.Draw(Position);
    SpriteBatch::SimpleDraw(HandTex, HandPos);
    Globals::DrawSinglePixelAtLoc(Position);

    // Debug bounds drawing
    DrawHeroBounds();
}

sf::FloatRect ETG::Hero::HeroBounds() const
{
    const auto& currTexRect = AnimationComp.CurrTexRect;
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
