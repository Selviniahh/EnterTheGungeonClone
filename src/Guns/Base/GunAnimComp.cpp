#include "GunAnimComp.h"

ETG::GunAnimComp::GunAnimComp()
{
    SetAnimations();
}

void ETG::GunAnimComp::SetAnimations()
{
}

void ETG::GunAnimComp::Update(const GunStateEnum& GunState, const AnimationKey& key)
{
    CurrentGunState = GunState;
    CurrentAnimState = key;

    auto& animManager = AnimManagerDict[CurrentGunState];
    animManager.Update(CurrentAnimState);

    const auto& animState = animManager.AnimationDict[CurrentAnimState];
    CurrTexRect = animState.CurrRect;
    CurrentTex = animState.GetCurrentFrameAsTexture();

    // Get the origin from the last key animation
    RelativeOrigin = animManager.AnimationDict[animManager.LastKey].Origin;
}

void ETG::GunAnimComp::Draw(const sf::Vector2f position, const sf::Vector2f Origin, const sf::Vector2f Scale, const float Rotation, const float depth)
{
    if (!AnimManagerDict.contains(CurrentGunState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");

    AnimManagerDict[CurrentGunState].Draw(CurrentTex, position, sf::Color::White, Rotation, Origin, Scale, depth);
}
