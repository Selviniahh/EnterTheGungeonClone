#include "Hero.h"
#include <filesystem>
#include "../Core/Components/CollisionComponent.h"
#include "../Core/Components/BaseHealthComp.h"
#include "../Enemy/EnemyBase.h"
#include "../Managers/GameState.h"
#include "../Managers/SpriteBatch.h"
#include "../Guns/RogueSpecial/RogueSpecial.h"
#include "../Projectile/ProjectileBase.h"
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
    HealthComp = ETG::CreateGameObjectAttached<BaseHealthComp>(this, 2.f); //by default health will be 4
    HealthComp->InvulnerabilityEnabled = false;

    //Collision comp:
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 1.f;
    CollisionComp->SetCollisionEnabled(true);

    //Set default gun to equipped guns
    EquippedGuns.push_back(RogueSpecial.get());
    CurrentGun = EquippedGuns[0]; //Hero's default gun is RogueSpecial
    ReloadText->LinkToGun(CurrentGun);

    Hero::Initialize();
}

void ETG::Hero::Initialize()
{
    GameObjectBase::Initialize();

    //NOTE: damage taken at here
    HealthComp->OnDamageTaken.AddListener([this](const float damage, const GameObjectBase* instigator)
    {
        //IF dead ignore the damage
        if (CurrentHeroState == HeroStateEnum::Die) return;

        CurrentHeroState = HeroStateEnum::Hit;
        const sf::Vector2f knockbackDir = Math::Normalize(Position - instigator->GetPosition());
        MoveComp->ApplyForce(knockbackDir, KnockBackMagnitude, 0.2f);
    });

    //NOTE: Hero will die at here
    HealthComp->OnDeath.AddListener([this](GameObjectBase* instigator)
    {
        CurrentHeroState = HeroStateEnum::Die;
    });

    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        //If the collision is with enemy, apply force to our hero and damage
        if (auto* enemyObj = dynamic_cast<EnemyBase*>(eventData.Other))
        {
            std::cout << "Collision with " << enemyObj->GetObjectName() << " Push yourself to opposite direction and damage yourself " << std::endl;
            KnockBackMagnitude = 350.f;
            HealthComp->ApplyDamage(0.5, enemyObj);
        }

        //If the collision is with enemy projectile, damage our hero and destroy the enemy projectile
        auto* enemyObj = dynamic_cast<EnemyBase*>(eventData.Other->Owner->Owner);
        auto* enemyProj = dynamic_cast<ProjectileBase*>(eventData.Other);
        if (enemyObj && enemyProj)
        {
            if (AnimationComp->IsDashing) return; //If dashing, ignore the hit and do not destroy the projectile

            KnockBackMagnitude = 150.f;
            HealthComp->ApplyDamage(0.5, enemyObj);
            enemyProj->MarkForDestroy();
        }

        //If it's ActiveItem, assign our pointer
        if (auto* activeItem = dynamic_cast<ActiveItemBase*>(eventData.Other))
        {
            CurrActiveItem = activeItem;
        }
    });

    CollisionComp->OnCollisionExit.AddListener([this](const CollisionEventData& eventData)
    {
        //No exit required for now
    });
}

ETG::Hero::~Hero() = default;

void ETG::Hero::Update()
{
    GameObjectBase::Update();
    CollisionComp->Update();
    InputComp->Update(*this);
    MoveComp->Update(); //NOTE: When InputComp changes `HeroPtr->CurrentHeroState` new AnimState changes needs to be reflected in `AnimationComp` then `MoveComp` or I can move all dash to AnimationComp????
    HealthComp->Update();

    // std::cout << HealthComp->CurrentHealth << std::endl;

    //Flip animations if not hit or dead
    if (!IsCurrStateHitDie()) AnimationComp->FlipSpritesY<GunBase>(CurrentDirection, *CurrentGun);
    if (!IsCurrStateHitDie()) AnimationComp->FlipSpritesX(CurrentDirection, *this);
    AnimationComp->Update();

    //Set hand properties
    const sf::Vector2f HandOffsetForHero = AnimationComp->IsFacingRight(CurrentDirection) ? sf::Vector2f{8.f, 5.f} : sf::Vector2f{-7.f, 5.f};
    Hand->SetPosition(Position + Hand->HandOffset + HandOffsetForHero);
    Hand->Update();

    //Will run only if reload needed 
    ReloadText->Update();

    //Gun orientation
    CurrentGun->SetPosition(Hand->GetPosition() + Hand->GunOffset);
    CurrentGun->Rotation = MouseAngle;


    //Shoot only if these conditions are met 
    if (IsShooting && CurrentGun->MagazineAmmo != 0 && !CurrentGun->IsReloading && !IsCurrStateDashHitDie())
    {
        CurrentGun->PrepareShooting();
    }

    //Try to use Active item
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && CurrActiveItem && !IsCurrStateDashHitDie())
    {
        CurrActiveItem->RequestUsage();
    }

    //If dashing or hit anim playing do not draw gun and hand
    Hand->IsVisible = !IsCurrStateDashHitDie();
    CurrentGun->IsVisible = !IsCurrStateDashHitDie();

    //Update  all equipped guns (for their projectiles only)
    //NOTE: if (IsAttachedObjectNeeded()) //Calling this will act like stopping the time for projectiles. If I had some time, I'd implement stop time active item
    for (const auto guns : EquippedGuns)
        guns->Update();

    GameObjectBase::Update();
}

void ETG::Hero::Draw()
{
    if (!IsVisible) return;
    GameObjectBase::Draw();
    SpriteBatch::Draw(GetDrawProperties());
    CurrentGun->Draw();
    ReloadText->Draw();
    Hand->Draw();

    //Draw all equipped guns (for their projectiles only)
    for (const auto guns : EquippedGuns)
        guns->Draw();

    if (CollisionComp) CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
}

bool ETG::Hero::IsCurrStateDashHitDie() const
{
    return CurrentHeroState == HeroStateEnum::Dash || CurrentHeroState == HeroStateEnum::Hit || CurrentHeroState == HeroStateEnum::Die;
}

bool ETG::Hero::IsCurrStateHitDie() const
{
    return CurrentHeroState == HeroStateEnum::Hit || CurrentHeroState == HeroStateEnum::Die;
}

//----------------------------Gun Switch Functionalities ----------------------------
ETG::GunBase* ETG::Hero::GetCurrentHoldingGun() const
{
    return CurrentGun;
}

void ETG::Hero::EquipGun(GunBase* newGun)
{
    EquippedGuns.push_back(newGun);
    CurrentGun = newGun; // Set the new gun as the current one by default
    currentGunIndex = EquippedGuns.size() - 1;
    ReloadText->LinkToGun(CurrentGun);
    UpdateGunVisibility();
}

void ETG::Hero::SwitchToPreviousGun()
{
    // First check if we have any guns at all
    if (EquippedGuns.empty()) return;

    // Move index backwards (-1) with wraparound
    // No need for additional bounds check - the modulo operation guarantees the index is valid if the vector is not empty
    currentGunIndex = (currentGunIndex - 1 + EquippedGuns.size()) % EquippedGuns.size();
    CurrentGun = EquippedGuns[currentGunIndex];
    ReloadText->LinkToGun(CurrentGun);
    ReloadText->SetNeedsReload(CurrentGun->IsMagazineEmpty());
    UpdateGunVisibility();
}

//Gun switching
void ETG::Hero::SwitchToNextGun()
{
    if (EquippedGuns.empty()) return;

    // Move index forwards with wraparound
    currentGunIndex = (currentGunIndex + 1) % EquippedGuns.size();
    CurrentGun = EquippedGuns[currentGunIndex];
    ReloadText->LinkToGun(CurrentGun);
    ReloadText->SetNeedsReload(CurrentGun->IsMagazineEmpty());
    UpdateGunVisibility();
}

void ETG::Hero::UpdateGunVisibility() const
{
    for (GunBase* gun : EquippedGuns)
    {
        gun->IsVisible = (gun == CurrentGun);
    }
}
