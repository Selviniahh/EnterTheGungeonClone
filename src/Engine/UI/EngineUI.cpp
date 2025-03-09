#include "EngineUI.h"
#include "../../Core/GameObjectBase.h"
#include <imgui.h>

#include "../../Managers/TypeRegistry.h"

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
    if (ImGui::InputFloat2("##PosX", &obj->Position.x, "%.3f"))
    {
    }

    //NOTE: Scale
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Scale");
    ImGui::SameLine();
    if (ImGui::InputFloat2("##Scale", &obj->Scale.x, "%.3f"))
    {
    }

    //NOTE: Rotation
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Rotation");
    ImGui::SameLine();
    ImGui::SliderFloat("##Rot", &obj->Rotation, 0.0f, 360.0f);
}

template <>
void ETG::ShowImGuiWidget<ETG::GameObjectBase*>(const char* label, GameObjectBase*& obj)
{
    if (!obj) return;

    if (ImGui::TreeNode(label))
    {
        TypeRegistry::ProcessObject(obj);
        ImGui::Spacing(); //Too much recursive visualization might get easily confusing
        ImGui::TreePop();
    }
}

template <>
void ETG::ShowImGuiWidget<bool>(const char* label, bool& value)
{
    BeginProperty(label);

    // Save current style
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));

    //make checkbox a bit narrower
    ImGui::PushItemWidth(5.f);

    if (ImGui::Checkbox("##value", &value))
    {
        // Value changed
    }

    // Restore style
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();

    EndProperty();
}

template <>
void ETG::ShowImGuiWidget<std::string>(const char* label, std::string& value)
{
    BeginProperty(label);
    char buffer[256] = {};
    std::strncpy(buffer, value.c_str(), sizeof(buffer) - 1);
    if (ImGui::InputText("##value", buffer, sizeof(buffer)))
    {
        value = buffer;
    }
    EndProperty();
}

template <>
void ETG::ShowImGuiWidget<std::shared_ptr<sf::Texture>>(const char* label, std::shared_ptr<sf::Texture>& value)
{
    BeginProperty(label);

    if (value)
    {
        // Get the native OpenGL texture handle from SFML
        const ImTextureID textId = value->getNativeHandle();

        float texWidth = static_cast<float>(value->getSize().x);
        float texHeight = static_cast<float>(value->getSize().y);

        ImGui::Text("%.0fx%.0f", texWidth, texHeight);

        //Scale the texture
        constexpr float MIN_DIMENSION = 64.0f;
        float scale = 1.0f;
        const float smallestDimension = std::max(texWidth, texHeight);
        while (smallestDimension * (scale + 1) < MIN_DIMENSION)
        {
            scale += 1.0f;
        }
        texWidth *= scale;
        texHeight *= scale;

        //Uv min 0,0 UV mac: 1 1 which means display full image
        ImVec2 uvMin(0.0f, 0.0f);
        ImVec2 uv_max(1.0f, 1.0f);

        ImGui::Image(textId, ImVec2(texWidth, texHeight), uvMin, uv_max);
    }
    else
    {
        ImGui::Text("No texture loaded");
    }

    EndProperty();
}

template <>
void ETG::ShowImGuiWidget<sf::Vector2<float>>(const char* label, sf::Vector2<float>& value)
{
    BeginProperty(label);
    ImGui::InputFloat2("##vector2", &value.x);
    EndProperty();
}

template <>
void ETG::ShowImGuiWidget<float>(const char* label, float& value)
{
    BeginProperty(label);

    ImGui::InputFloat("##value", &value);

    EndProperty();
}


void ETG::BeginProperty(const char* label)
{
    ImGui::PushID(label);
    ImGui::Columns(2);

    ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.40f);

    // Reduce vertical spacing
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, ImGui::GetStyle().ItemSpacing.y * 0.8f));

    ImGui::AlignTextToFramePadding();

    ImGui::Text("%s", label);

    ImGui::NextColumn();
    ImGui::SetNextItemWidth(-1);
}

void ETG::EndProperty()
{
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();

    // Reduce spacing between properties
    ImGui::Spacing();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2); // Slightly reduce space between items
}
