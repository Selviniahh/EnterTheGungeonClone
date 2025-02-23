#include "GameObject.h"
#include "../Managers/GameState.h"


GameObjectBase::GameObjectBase()
{
    ETG::GameState::GetInstance().GetSceneObj().push_back(this);
}

void GameObjectBase::ComputeDrawProperties()
{
    DrawProps.Position = Position + RelativePos;
    DrawProps.Scale = {Scale.x * RelativeScale.x, Scale.y * RelativeScale.y};
    DrawProps.Rotation = Rotation + RelativeRotation;
    DrawProps.Origin = Origin + RelativeOrigin;
}