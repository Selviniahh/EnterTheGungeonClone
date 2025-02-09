//
// Created by Selviniah on 2025-02-08.
//

#include "HealthComp.h"

HealthComp::HealthComp(const float MaxHealth) : Damaged(false), Timer(0), IsDead(false), MaxHealth(MaxHealth)
{
}

void HealthComp::Initialize()
{
}

void HealthComp::DamageTaken(GameObject* DamagedActor, float Damage)
{
}
