#pragma once
#include "../../Core/GameObjectBase.h"

namespace ETG
{
    class ComponentBase;

    class ActiveItemBase : GameObjectBase
    {
        ActiveItemBase()
        {
        }

        ~ActiveItemBase() override = default;

        BOOST_DESCRIBE_CLASS(ActiveItemBase, (GameObjectBase),(),(),())
    };
}
