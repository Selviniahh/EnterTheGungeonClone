#include "GunAnimComp.h"

ETG::GunAnimComp::GunAnimComp()
{
    SetAnimations();
}

void ETG::GunAnimComp::Update(const GunStateEnum& GunState, const AnimationKey& animState)
{
    CurrentGunState = GunState;
    CurrentAnimState = animState;

    auto& currAnimManager = AnimManagerDict[CurrentGunState];
    currAnimManager.Update(CurrentAnimState);

    const auto& currAnimState = currAnimManager.AnimationDict[CurrentAnimState];
    CurrTexRect = currAnimState.CurrRect;
    CurrentTex = currAnimState.GetCurrentFrameAsTexture();
    RelativeOrigin = currAnimManager.AnimationDict[AnimManagerDict[CurrentGunState].LastKey].Origin;
}
void ETG::GunAnimComp::Draw(const sf::Vector2f position, const sf::Vector2f Origin, const sf::Vector2f Scale, const float Rotation, const float depth)
{
    if (!AnimManagerDict.contains(CurrentGunState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");
            
    AnimManagerDict[CurrentGunState].Draw(CurrentTex, position, sf::Color::White,Rotation, Origin,Scale, depth);
}



void ETG::GunAnimComp::SetAnimations()
{
    //Idle Animation
    const auto IdleAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial", "RogueSpecial_Idle", "PNG", 0.15f, true);
    auto IdleAnimManager = AnimationManager();
    IdleAnimManager.AddAnimation(GunStateEnum::Idle, IdleAnim);
    IdleAnimManager.SetOrigin(GunStateEnum::Idle, sf::Vector2f{(float)(IdleAnim.FrameRects[0].width / 2), (float)(IdleAnim.FrameRects[0].width / 2),});

    AnimManagerDict[GunStateEnum::Idle] = IdleAnimManager;

    //Shoot animations
    const auto ShootAnims = std::vector<Animation>{
        Animation::CreateSpriteSheet("Guns/RogueSpecial/Fire", "knav3_fire_001", "PNG", 0.15f),
    };

    auto ShootAnimManager = AnimationManager();
    for (int i = 0; i < ShootAnims.size(); ++i)
    {
        ShootAnimManager.AddAnimation(GunStateEnum::Shoot, ShootAnims[i]);
        ShootAnimManager.SetOrigin(GunStateEnum::Shoot, sf::Vector2f{(float)(ShootAnims[i].FrameRects[0].width / 2), (float)(ShootAnims[i].FrameRects[0].width / 2),});
    }
    
    AnimManagerDict[GunStateEnum::Shoot] = ShootAnimManager;

    //Reload Animation
    const auto ReloadAnim = Animation::CreateSpriteSheet("Guns/RogueSpecial", "RogueSpecial_Reload", "PNG", 0.15f, true);
    auto ReloadAnimManager = AnimationManager();
    ReloadAnimManager.AddAnimation(GunStateEnum::Reload, ReloadAnim);
    ReloadAnimManager.SetOrigin(GunStateEnum::Reload, sf::Vector2f{(float)(ReloadAnim.FrameRects[0].width / 2), (float)(ReloadAnim.FrameRects[0].width / 2),});

    AnimManagerDict[GunStateEnum::Reload] = ReloadAnimManager;
}
