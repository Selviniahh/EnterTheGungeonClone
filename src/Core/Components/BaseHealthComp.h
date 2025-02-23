#pragma once
#include "../GameObject.h"

class GameObject;

class BaseHealthComp : GameObject {
public:
    explicit BaseHealthComp(float MaxHealth);
    void Initialize() override;

    bool Damaged;
    float Timer;
    bool IsDead;
    float Health;

private:
    static void DamageTaken(GameObject* DamagedActor, float Damage);

    float MaxHealth;
};



