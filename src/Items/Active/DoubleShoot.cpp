#include "DoubleShoot.h"
#include <filesystem>
#include "../../Core/Components/CollisionComponent.h"
#include "../../Characters/Hero.h"
#include "../../Core/Factory.h"
#include "../../Guns/Base/GunBase.h"
#include "../../Modifiers/Gun/MultiShotModifier.h"

ETG::DoubleShoot::DoubleShoot() : ActiveItemBase((std::filesystem::path(RESOURCE_PATH) / "Items" / "Active" / "Potion_of_Gun_Friendship.png").generic_string(),
(std::filesystem::path(RESOURCE_PATH) / "Sounds" / "Consume.ogg").generic_string(),
    DEFAULT_COOLDOWN, DEFAULT_ACTIVE_TIME)
{
    ItemDescription = "Double shoot the item and set Spread 0";
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 15.f;
    CollisionComp->SetCollisionEnabled(true);
    Position = {100, -70};

    Origin = sf::Vector2f{(float)Texture->getSize().x / 2, (float)Texture->getSize().y / 2};

    DoubleShoot::Initialize();
}

void ETG::DoubleShoot::Initialize()
{
    ActiveItemBase::Initialize();
    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        //If collided object is hero:  
        if (auto* heroObj = dynamic_cast<Hero*>(eventData.Other))
        {
            Owner = dynamic_cast<GameObjectBase*>(heroObj); //In UI move from scene to Hero
            if (!IsVisible) return;

            PlayRandomPickupSound();

            //Add self to equipped passive items list
            GameState::GetInstance().GetEquippedActiveItems().push_back(this);
        }
    });
}

void ETG::DoubleShoot::Update()
{
    ActiveItemBase::Update();

    // Handle deactivation.
    // NOTE: During cooldown this will continusely run. Since the array is not big, in every tick during cooldown stage, trying to find an element in vector is not a problem but will be sooner or later
    //NOTE: So I should define WasConsumingLastTick in ActiveItemState set that and use in this if block so that this if block will only run once  
    if (ActiveItemState == ActiveItemState::Cooldown)
    {
        // Remove the modifier when effect ends
        const auto hero = GameState::GetInstance().GetHero();
        const auto gun = hero->GetCurrentHoldingGun();
        if (gun->modifierManager.HasModifier("MultiShot"))
        {
            gun->modifierManager.RemoveModifier("MultiShot");
        }
    }
}
    
void ETG::DoubleShoot::Draw()
{
    ActiveItemBase::Draw();
    CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
}

void ETG::DoubleShoot::RequestUsage()
{
    // Only allow usage if the cooldown is complete
    if (ActiveItemState == ActiveItemState::Ready)
    {
        ActivateSound.play();

        // Reset state
        ActiveItemState = ActiveItemState::Consuming;
        ConsumeTimer = 0;

        // Apply the double-shoot effect
        const auto hero = GameState::GetInstance().GetHero();
        ApplyPerk(hero);
    }
}

void ETG::DoubleShoot::ApplyPerk(const Hero* hero)
{
    //Create the modifier and push back the array inside modifierManager
    hero->GetCurrentHoldingGun()->modifierManager.AddModifier(std::make_shared<MultiShotModifier>(ShootCount,SpreadAmount));
}
