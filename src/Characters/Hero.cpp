#include "Hero.h"
#include <iostream>
#include "../Managers/InputManager.h"

sf::Vector2f ETG::Hero::HeroPosition = {0.f, 0.f};
float ETG::Hero::MouseAngle = 0;
ETG::Direction ETG::Hero::CurrentDirection{};

ETG::Hero::Hero(const sf::Vector2f Position) : HandPos({}), HandTex({})
{
    HeroPosition = Position;
    if (!HandTex.loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Player" / "rogue_hand_001.PNG").string()))
        std::cerr << "Failed to load hand texture" << std::endl;
}

void ETG::Hero::Update()
{
    AnimationKey animState = IdleEnum::Idle_Back;
    
    InputComp.Update(*this);
    MoveComp.UpdateMovement(*this);
    
    if (CurrentHeroState == HeroStateEnum::Dash)
    {
        animState = InputComponent::GetDashDirectionEnum();
    }
    else if (CurrentHeroState == HeroStateEnum::Run)
    {
        animState = GetRunEnum(CurrentDirection);
    }
    else if (CurrentHeroState == HeroStateEnum::Idle)
    {
        animState = GetIdleDirectionEnum(CurrentDirection);
    }

    AnimationComp.Update(CurrentHeroState, animState);
    RelativeHandLoc = AnimationComp.FlipSprites(CurrentDirection);
    SetHandTexLoc();
}

void ETG::Hero::Draw()
{
    AnimationComp.Draw(HeroPosition);
    Globals::Renderer::SimpleDraw(HandTex, HandPos);
    Globals::DrawSinglePixelAtLoc(HeroPosition);

    // Debug bounds drawing
    DrawHeroBounds();
}

sf::FloatRect ETG::Hero::HeroBounds() const
{
    const auto& currTexRect = AnimationComp.CurrTexRect;
    return {
        HeroPosition.x - currTexRect.width / 2.f,
        HeroPosition.y - currTexRect.height / 2.f,
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
    HandPos = HeroPosition + RelativeHandLoc;
    HandPos.x -= static_cast<float>(HandTex.getSize().x) / 2;
    HandPos.y -= static_cast<float>(HandTex.getSize().y) / 2;
}

ETG::RunEnum ETG::Hero::GetRunEnum(const Direction currDir)
{
    if (currDir == Direction::BackHandRight || currDir == Direction::BackHandLeft) return RunEnum::Run_Back;
    if (currDir == Direction::BackDiagonalRight || currDir == Direction::BackDiagonalLeft) return RunEnum::Run_BackWard;
    if (currDir == Direction::Right || currDir == Direction::Left) return RunEnum::Run_Forward;
    if (currDir == Direction::FrontHandRight || currDir == Direction::FrontHandLeft) return RunEnum::Run_Front;
    return RunEnum::Run_Forward; // Default case
}

ETG::IdleEnum ETG::Hero::GetIdleDirectionEnum(const Direction currDir)
{
    if (currDir == Direction::BackHandRight || currDir == Direction::BackHandLeft) return IdleEnum::Idle_Back;
    if (currDir == Direction::BackDiagonalRight || currDir == Direction::BackDiagonalLeft) return IdleEnum::Idle_BackWard;
    if (currDir == Direction::Right || currDir == Direction::Left) return IdleEnum::Idle_Right;
    if (currDir == Direction::FrontHandRight || currDir == Direction::FrontHandLeft) return IdleEnum::Idle_Front;
    return IdleEnum::Idle_Back; // Default case
}
