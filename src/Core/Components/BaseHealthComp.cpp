//
// Created by Selviniah on 2025-02-08.
//

#include "BaseHealthComp.h"

BaseHealthComp::BaseHealthComp(const float MaxHealth) : Damaged(false), Timer(0), IsDead(false), MaxHealth(MaxHealth)
{
}

void BaseHealthComp::Initialize()
{
}

void BaseHealthComp::DamageTaken(GameObjectBase* DamagedActor, float Damage)
{
}
