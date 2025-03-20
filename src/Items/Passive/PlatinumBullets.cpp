#include <iostream>
#include <filesystem>
#include "../../Core/Components/CollisionComponent.h"
#include "PlatinumBullets.h"
#include "../../Characters/Hero.h"
#include "../../Core/Factory.h"
#include "../../Guns/Base/GunBase.h"

ETG::PlatinumBullets::PlatinumBullets()
{
    ItemDescription = "Increase the fire rate %20";
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 15.f;
    CollisionComp->SetCollisionEnabled(true);
    Position = {100,-10};

    PlatinumBullets::Initialize();
}

void ETG::PlatinumBullets::Initialize()
{
    PassiveItemBase::Initialize();

    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        if (const auto* heroObj = dynamic_cast<Hero*>(eventData.Other))
        {
            if (!IsVisible) return;

            //Play a random pickup sound when collision occurs
            std::uniform_int_distribution<int> dist(0, Sounds.size() -1);
            const int soundIndex = dist(rng);
            Sounds[soundIndex].play();
            IsVisible = false;

            //Apply the perk
            Perk(heroObj);
        }
    });
}

void ETG::PlatinumBullets::Update()
{
    PassiveItemBase::Update();
    CollisionComp->Update();
}

void ETG::PlatinumBullets::Draw()
{
    PassiveItemBase::Draw();
    CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
}

void ETG::PlatinumBullets::Perk(const Hero* hero)
{
    //Decrease the Fire rate by %20
    hero->GetCurrentHoldingGun()->FireRate = hero->GetCurrentHoldingGun()->FireRate * 0.8;
}