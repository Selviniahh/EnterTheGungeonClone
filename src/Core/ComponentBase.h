#pragma once
#include "GameObjectBase.h"

namespace ETG
{
    //For now none of the components needs to be drawable so it's unnecessary 
    class ComponentBase : public GameObjectBase
    {
    public:
        ComponentBase()
        {
        }

        ~ComponentBase() override = default;

        BOOST_DESCRIBE_CLASS(ComponentBase,(GameObjectBase),(),(),())
    };
}
