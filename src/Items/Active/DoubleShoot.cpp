#include <filesystem>
#include "../../Core/Components/CollisionComponent.h"
#include "../../Characters/Hero.h"
#include "../../Core/Factory.h"
#include "../../Guns/Base/GunBase.h"

#include "DoubleShoot.h"

ETG::DoubleShoot::DoubleShoot() : ActiveItemBase((std::filesystem::path(RESOURCE_PATH) / "Items" / "Active" / "Potion_of_Gun_Friendship.png").generic_string())
{
    ItemDescription = "Increase the fire rate %20";
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 15.f;
    CollisionComp->SetCollisionEnabled(true);
    Position = {100, -70};
    
    DoubleShoot::Initialize();
}

void ETG::DoubleShoot::Initialize()
{
    ActiveItemBase::Initialize();
    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
       if (const auto* heroObj = dynamic_cast<Hero*>(eventData.Other))
       {
           if (!IsVisible) return;

           PlayRandomPickupSound();

           //Apply the perk
           Perk(heroObj);

           //Add self to equipped passive items list
           GameState::GetInstance().GetEquippedActiveItems().push_back(this);
           
       }
    });
}

void ETG::DoubleShoot::Update()
{
    ActiveItemBase::Update();
    CollisionComp->Update();

}

void ETG::DoubleShoot::Draw()
{
    ActiveItemBase::Draw();
    CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
}

void ETG::DoubleShoot::Perk(const Hero* hero)
{
    hero->GetCurrentHoldingGun()->AddModifier(std::make_shared<MultiShotModifier>(2));
}
