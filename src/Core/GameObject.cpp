#include "GameObject.h"
#include "../Managers/GameState.h"
#include "../Core/Factory.h"

GameObject::GameObject()
{
    ETG::GameState::GetInstance().GetSceneObj().push_back(this);
    SetObjectNameToSelfClassName();
}

void GameObject::ComputeDrawProperties()
{
    DrawProps.Position = Position + RelativePos;
    DrawProps.Scale = {Scale.x * RelativeScale.x, Scale.y * RelativeScale.y};
    DrawProps.Rotation = Rotation + RelativeRotation;
    DrawProps.Origin = Origin + RelativeOrigin;
}

void GameObject::SetObjectNameToSelfClassName()
{
    ObjectName = boost::typeindex::type_id_runtime(*this).pretty_name();

    // //Remove everything up to and including the last ::
    const size_t LastColon = ObjectName.find_last_of("::");
    if (LastColon != std::string::npos)
    {
        ObjectName = ObjectName.substr(LastColon + 1);
    }
}
