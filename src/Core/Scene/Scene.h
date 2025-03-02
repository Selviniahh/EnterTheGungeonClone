#pragma once
#include "../GameObjectBase.h"

namespace ETG
{
    class Scene : public GameObjectBase
    {
    public:
        Scene();
        ~Scene() override = default;
    };

    inline Scene::Scene()
    {
    }
    
}
