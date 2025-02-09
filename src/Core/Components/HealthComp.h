#pragma once

class GameObject;

class HealthComp {
public:
    explicit HealthComp(float MaxHealth);
    static void Initialize();

    bool Damaged;
    float Timer;
    bool IsDead;
    float Health;

private:
    static void DamageTaken(GameObject* DamagedActor, float Damage);

    float MaxHealth;
};



