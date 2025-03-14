#include "ActiveItem.h"
#include <iostream>
#include "../Managers/Globals.h"
#include "../Characters/Hero.h"


ActiveItem::ActiveItem(const std::string& texturePath)
{
    Texture = std::make_shared<sf::Texture>();
    if (!Texture->loadFromFile(texturePath))
    {
        std::cerr << "Failed to load active item texture: " << texturePath << std::endl;
    }
    equipSoundBuffer.loadFromFile("Resources/Sounds/equip_sound.wav");
    equipSound.setBuffer(equipSoundBuffer);
}

void ActiveItem::Initialize()
{
    Origin.x = Texture->getSize().x / 2;
    Origin.y = Texture->getSize().y / 2;
}

void ActiveItem::Update()
{
    if (isConsuming)
    {
        ;
        consumptionTime -= ETG::Globals::FrameTick;
        if (consumptionTime <= 0)
        {
            isConsuming = false;
            isCooldown = true;
            cooldownTime = 10.0f;
        }
    }
    if (isCooldown)
    {
        cooldownTime -= ETG::Globals::FrameTick;
        if (cooldownTime <= 0)
        {
            isCooldown = false;
        }
    }
}

void ActiveItem::Draw() {
    if (Texture) {
        // Access hero through GameState
        ETG::Hero* hero = ETG::GameState::GetInstance().GetHero();
        if (hero && hero->IsMouseNearHero(*ETG::Globals::Window)) {
            sf::Sprite sprite;
            sprite.setTexture(*Texture);
            sprite.setScale(0.5f, 0.5f);
            sprite.setPosition(GetPosition());
            ETG::Globals::Window->draw(sprite);
        }
    } else {
        std::cerr << "ActiveItem texture is null!" << std::endl;
    }
}

void ActiveItem::Use()
{
    if (!isConsuming && !isCooldown)
    {
        isConsuming = true;
        consumptionTime = 5.0f;
    }
}

bool ActiveItem::IsConsuming() const
{
    return isConsuming;
}

bool ActiveItem::IsOnCooldown() const
{
    return isCooldown;
}

void ActiveItem::PlayEquipSound()
{
    equipSound.play();
}

// Implement the new methods
float ActiveItem::GetConsumptionProgress() const
{
    return consumptionTime / 5.0f; // Normalize to a value between 0 and 1
}

float ActiveItem::GetCooldownProgress() const
{
    return cooldownTime / 10.0f; // Normalize to a value between 0 and 1
}
