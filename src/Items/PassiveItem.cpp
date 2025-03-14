#include "PassiveItem.h"

#include <filesystem>
#include <iostream>

PassiveItem::PassiveItem(const std::string& texturePath) {
    Texture = std::make_shared<sf::Texture>();
    if (!Texture->loadFromFile(texturePath)) {
        std::cerr << "Failed to load passive item texture: " << texturePath << std::endl;
    }
    equipSoundBuffer.loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "equip_sound.wav").generic_string());
    equipSound.setBuffer(equipSoundBuffer);
}

void PassiveItem::Initialize() {
    Origin.x = Texture->getSize().x / 2;
    Origin.y = Texture->getSize().y / 2;
}
void PassiveItem::Update(float deltaTime) {
    // No update logic needed for passive items
}
void PassiveItem::Draw() {
    ETG::GameObjectBase::Draw(); // Draw the base texture
}

void PassiveItem::PlayEquipSound() {
    equipSound.play();
}