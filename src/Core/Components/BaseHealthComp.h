#pragma once

class GameObject;

class BaseHealthComp {
public:
    explicit BaseHealthComp(float MaxHealth);
    static void Initialize();

    bool Damaged;
    float Timer;
    bool IsDead;
    float Health;

private:
    static void DamageTaken(GameObject* DamagedActor, float Damage);

    float MaxHealth;
};



