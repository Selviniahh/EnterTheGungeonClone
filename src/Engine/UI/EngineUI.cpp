//
// Created by selviniah on 23/02/25.
//

#include "EngineUI.h"
#include "../../Core/GameObject.h"
#include <imgui.h>

void ImGuiSetRelativeOrientation(GameObjectBase* obj)
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
void ImGuiSetAbsoluteOrientation(GameObjectBase* obj)
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