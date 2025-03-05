#include "EngineUI.h"
#include "../../Core/GameObjectBase.h"
#include <imgui.h>

void ETG::ImGuiSetRelativeOrientation(GameObjectBase* obj)
{
    //NOTE: Position
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Position");
    ImGui::SameLine();
    if (ImGui::InputFloat2("##PosX", &obj->RelativePos.x, "%.3f"))
    {
        //Revert what I've done last time and then apply RelativePos again
        const sf::Vector2f delta = {obj->RelativePos.x - obj->PrevRelativePos.x, obj->RelativePos.y - obj->PrevRelativePos.y};
        obj->DrawProps.Position += delta;
        obj->PrevRelativePos = obj->RelativePos;
    }

    //NOTE: Scale
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Scale");
    ImGui::SameLine();
    if (ImGui::InputFloat2("##Scale", &obj->RelativeScale.x, "%.3f"))
    {
        const sf::Vector2f delta{obj->RelativeScale.x - obj->PrevRelativeScale.x, obj->RelativeScale.y - obj->PrevRelativeScale.y};
        obj->DrawProps.Scale += delta;
        obj->PrevRelativeScale = obj->RelativeScale;
    }

    //NOTE: Rotation
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Rotation");
    ImGui::SameLine();
    ImGui::SliderFloat("##Rot", &obj->RelativeRotation, 0.0f, 360.0f);
}

// When the UI changes, recalculate the final position from BasePosition.
void ETG::ImGuiSetAbsoluteOrientation(GameObjectBase* obj)
{
    //NOTE: Position
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Position");
    ImGui::SameLine();
    if (ImGui::InputFloat2("##PosX", &obj->Position.x, "%.3f")) {}

    //NOTE: Scale
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Scale");
    ImGui::SameLine();
    if (ImGui::InputFloat2("##Scale", &obj->Scale.x, "%.3f")) {}

    //NOTE: Rotation
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Rotation");
    ImGui::SameLine();
    ImGui::SliderFloat("##Rot", &obj->Rotation, 0.0f, 360.0f);
}

template <>
void ETG::ShowImGuiWidget<ETG::GameObjectBase*>(const char* label, GameObjectBase*& value)
{
    ImGui::Text("This is Game Object base which is owner of the object. I need to decide how to display Game Object in UI");
}

template <>
void ETG::ShowImGuiWidget<bool>(const char* label, bool& value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label);
    ImGui::SameLine();
    if (ImGui::Checkbox(label,&value))
    {
        
    };
}

template <>
void ETG::ShowImGuiWidget<std::string>(const char* label, std::string& value)
{
    
}

template <>
void ETG::ShowImGuiWidget<std::shared_ptr<sf::Texture>>(const char* label, std::shared_ptr<sf::Texture>& value)
{
    
}

template <>
void ETG::ShowImGuiWidget<sf::Vector2<float>>(const char* label, sf::Vector2<float>& value)
{
    
}

template <>
void ETG::ShowImGuiWidget<float>(const char* label, float& value)
{
    
}
