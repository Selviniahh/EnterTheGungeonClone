#pragma once
#include "../GameObjectBase.h"

namespace ETG
{
    class Scene : public GameObjectBase
    {
    public:
        Scene();
        ~Scene() override = default;

    private:
        BOOST_DESCRIBE_CLASS(Scene,(GameObjectBase),
            (),
            (),
            ())
    };

    inline Scene::Scene()
    {
    }
    
}
