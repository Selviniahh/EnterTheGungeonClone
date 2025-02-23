#include "GameObject.h"
#include "../Managers/GameState.h"

GameObject::GameObject()
{
    ETG::GameState::GetInstance().GetSceneObj().push_back(this);
}

void GameObject::ComputeDrawProperties()
{
    DrawProps.Position = Position + RelativePos;
    DrawProps.Scale = {Scale.x * RelativeScale.x, Scale.y * RelativeScale.y};
    DrawProps.Rotation = Rotation + RelativeRotation;
    DrawProps.Origin = Origin + RelativeOrigin;
}
