#pragma once
#include "../GameObject.h"

class BaseHealthComp : ETG::GameObject {
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



