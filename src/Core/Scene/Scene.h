#pragma once
#include "../GameObject.h"

namespace ETG
{
    class Scene : public GameObject
    {
    public:
        Scene();
        ~Scene() override = default;
    };

    inline Scene::Scene()
    {
    }
    
}
