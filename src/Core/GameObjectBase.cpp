#include "GameObjectBase.h"
#include "../Managers/GameState.h"
#include <boost/type_index.hpp>
#include <imgui.h>
#include <iostream>
#include "../Managers/SpriteBatch.h"
#include "../Utils/StrManipulateUtil.h"


ETG::GameObjectBase::GameObjectBase()
{
}

ETG::GameObjectBase::~GameObjectBase()
{
    //Since the pointer doesn't own the memory, I don't need to bother with memory leak for the pointer. However just in case I set it to nullptr in destructor
    AnimInterface = nullptr;
}

void ETG::GameObjectBase::Update()
{
    //Calculate the final drawing properties. The Base position modified from the source and relative pos given in the UI will be summed to form the final value before being drawn
    ComputeDrawProperties();
}

void ETG::GameObjectBase::ComputeDrawProperties()
{
    DrawProps.Position = Position + RelativePos;
    DrawProps.Scale = {Scale.x * RelativeScale.x, Scale.y * RelativeScale.y};
    DrawProps.Rotation = Rotation + RelativeRotation;
    DrawProps.Origin = Origin + RelativeOrigin;
}

sf::FloatRect ETG::GameObjectBase::GetBounds() const
{
    if (AnimInterface)
    {
        // Use animation component's texture rect for bounds
        const auto& texRect = AnimInterface->GetCurrentTextureRect();
        return {
            Position.x - texRect.width / 2.f,
            Position.y - texRect.height / 2.f,
            static_cast<float>(texRect.width),
            static_cast<float>(texRect.height)
        };
    }
    else if (Texture)
    {
        // Fallback to using the texture size if available
        return {
            Position.x - Texture->getSize().x / 2.f,
            Position.y - Texture->getSize().y / 2.f,
            static_cast<float>(Texture->getSize().x),
            static_cast<float>(Texture->getSize().y)
        };
    }

    // Default to a small square if no texture information is available
    return {
        Position.x - 5.f,
        Position.y - 5.f,
        10.f,
        10.f
    };
}

void ETG::GameObjectBase::DrawBounds(sf::Color color) const
{
    GlobSpriteBatch.drawRectOutline(GetBounds(), color, 1.0f, 0);
}

std::string ETG::GameObjectBase::SetObjectNameToSelfClassName()
{
    ObjectName = boost::typeindex::type_id_runtime(*this).pretty_name();
    RemoveNamespaceFromName(ObjectName);

    TypeName = boost::typeindex::type_id<decltype(*this)>().pretty_name();
    RemoveNamespaceFromName(TypeName);

    IncrementName();

    return ObjectName;
}

//It feels good to write everything myself 
void ETG::GameObjectBase::IncrementName()
{
    const auto& SceneObjs = ETG::GameState::GetInstance().GetSceneObjs();
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
                std::string Suffix = objectName.substr(ObjectName.size());
                const signed int LastDigit = std::stoi(Suffix);
                Suffixes.push_back(LastDigit);
            }
        }

        const int max = std::ranges::max(Suffixes);
        ObjectName = BaseName + std::to_string(max + 1);
    }
}
