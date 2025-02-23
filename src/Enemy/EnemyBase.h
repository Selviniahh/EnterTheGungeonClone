#pragma once
#include "../Core/GameObject.h"

class EnemyBase : GameObject<EnemyBase>
{
protected:
    void Initialize() override;
    void Update() override;
    void Draw() override;

    
    
};
