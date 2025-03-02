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
            IsDrawable = false;
        }

        ~ComponentBase() override = default;
    };
}
