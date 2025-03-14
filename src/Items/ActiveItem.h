#ifndef ACTIVEITEM_H
#define ACTIVEITEM_H

#include "../Core/GameObjectBase.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ActiveItem : public ETG::GameObjectBase {
public:
     ActiveItem(const std::string& texturePath);
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Use();
    bool IsConsuming() const;
    bool IsOnCooldown() const;
    void PlayEquipSound();

    float GetConsumptionProgress() const;
    float GetCooldownProgress() const;

private:
    float consumptionTime = 5.0f; // Time the effect lasts
    float cooldownTime = 10.0f;   // Cooldown after use
    bool isConsuming = false;
    bool isCooldown = false;
    sf::Sound equipSound;
    sf::SoundBuffer equipSoundBuffer;
};

#endif