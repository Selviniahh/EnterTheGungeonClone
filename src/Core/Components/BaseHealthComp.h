#pragma once
#include "../ComponentBase.h"

class BaseHealthComp : ETG::ComponentBase {
public:
    explicit BaseHealthComp(float MaxHealth);
    void Initialize() override;

    bool Damaged;
    float Timer;
    bool IsDead;
    float Health;

private:
    static void DamageTaken(GameObjectBase* DamagedActor, float Damage);

    float MaxHealth;
};



