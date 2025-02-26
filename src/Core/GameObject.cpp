#include "GameObject.h"
#include "../Managers/GameState.h"
#include <boost/type_index.hpp>
#include <imgui.h>
#include <iostream>


GameObject::GameObject()
{
}

void GameObject::ComputeDrawProperties()
{
    DrawProps.Position = Position + RelativePos;
    DrawProps.Scale = {Scale.x * RelativeScale.x, Scale.y * RelativeScale.y};
    DrawProps.Rotation = Rotation + RelativeRotation;
    DrawProps.Origin = Origin + RelativeOrigin;
}

std::string GameObject::SetObjectNameToSelfClassName()
{
    ObjectName = boost::typeindex::type_id_runtime(*this).pretty_name();

    // //Remove everything up to and including the last ::
    const size_t LastColon = ObjectName.find_last_of("::");
    if (LastColon != std::string::npos)
    {
        ObjectName = ObjectName.substr(LastColon + 1);
    }

    IncrementName();


    return ObjectName;
}

//It feels good to write everything myself 
void GameObject::IncrementName()
{
    const auto& SceneObjs = ETG::GameState::GetInstance().GetSceneObj();
    const std::string BaseName = ObjectName;

    if (SceneObjs.contains(BaseName))
    {
        std::vector<signed int> Suffixes;
        Suffixes.push_back(1);

        for (const auto& [objectName, gameObject] : SceneObjs)
        {
            if (objectName == ObjectName) continue; //Current element is the first object initialized

            if (objectName.contains(BaseName)) //Basename is found, Get this obj's suffix value 
            {
                std::string Suffix =  objectName.substr(ObjectName.size());
                const signed int LastDigit = std::stoi(Suffix);
                Suffixes.push_back(LastDigit);
            }
        }

        const int max = std::ranges::max(Suffixes);
        ObjectName = BaseName + std::to_string(max + 1);
    }
    
    
}
