#include "ActiveItem.h"
#include <iostream>

ActiveItem::ActiveItem(const std::string& texturePath) {
    Texture = std::make_shared<sf::Texture>();
    if (!Texture->loadFromFile(texturePath)) {
        std::cerr << "Failed to load active item texture: " << texturePath << std::endl;
    }
    equipSoundBuffer.loadFromFile("Resources/Sounds/equip_sound.wav");
    equipSound.setBuffer(equipSoundBuffer);
}

void ActiveItem::Initialize() {
    Origin.x = Texture->getSize().x / 2;
    Origin.y = Texture->getSize().y / 2;
}

void ActiveItem::Update(float deltaTime) {
    if (isConsuming) {
        consumptionTime -= deltaTime;
        if (consumptionTime <= 0) {
            isConsuming = false;
            isCooldown = true;
            cooldownTime = 10.0f;
        }
    }
    if (isCooldown) {
        cooldownTime -= deltaTime;
        if (cooldownTime <= 0) {
            isCooldown = false;
        }
    }
}

void ActiveItem::Draw() {
    ETG::GameObjectBase::Draw(); // Draw the base texture
}

void ActiveItem::Use() {
    if (!isConsuming && !isCooldown) {
        isConsuming = true;
        consumptionTime = 5.0f;
    }
}

bool ActiveItem::IsConsuming() const {
    return isConsuming;
}

bool ActiveItem::IsOnCooldown() const {
    return isCooldown;
}

void ActiveItem::PlayEquipSound() {
    equipSound.play();
}

// Implement the new methods
float ActiveItem::GetConsumptionProgress() const {
    return consumptionTime / 5.0f; // Normalize to a value between 0 and 1
}

float ActiveItem::GetCooldownProgress() const {
    return cooldownTime / 10.0f; // Normalize to a value between 0 and 1
}