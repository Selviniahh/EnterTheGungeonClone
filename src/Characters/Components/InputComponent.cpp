#include "InputComponent.h"
#include <imgui.h>

#include "HeroAnimComp.h"
#include "HeroMoveComp.h"
#include "../Hero.h"
#include "../../Managers/InputManager.h"
#include "../../Utils/DirectionUtils.h"
#include "../../Utils/Math.h"
#include "../../Utils/StrManipulateUtil.h"

//This class will set `HeroPtr->CurrentHeroState`
//HeroAnimComp will set real animation based on HeroPtr->CurrentHeroState

namespace ETG
{
    InputComponent::InputComponent()
    {
        IsGameObjectUISpecified = true;
        DirectionUtils::PopulateDirectionRanges(DirectionMap);
        // SetObjectNameToSelfClassName();
    }

    void InputComponent::Update(Hero& hero) const
    { 
        UpdateDirection(hero);

        //If right-clicked, start dashing
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !hero.AnimationComp->IsDashing)
        {
            const HeroDashEnum dashDirection = DirectionUtils::GetDashDirectionEnum(); //If you put breakpoint this line, direction enum will always be unknown so put breakpoint below this line to capture dash direction
            if (dashDirection != HeroDashEnum::Unknown)
            {
                hero.AnimationComp->StartDash(dashDirection);
            }
        }
    }

    void InputComponent::UpdateDirection(Hero& hero) const
    {
        // Convert mouse angle to [0..360)
        float angle = Math::RadiansToDegrees(InputManager::GetMouseAngleRelativeToHero());
        if (angle < 0.f) angle += 360.f;

        // Store on the Hero (or handle in input component)
        hero.MouseAngle = angle;

        //NOTE: If it's Dash Set Hero's Direction based on the Keyboard Key
        if (hero.CurrentHeroState == HeroStateEnum::Dash)
        {
            hero.CurrentDirection = DirectionUtils::GetDirectionFromDash();
        }
        else //NOTE: If it's not Dash, Hero's set Hero's input based on Mouse Angle. 
        {
            hero.CurrentDirection = DirectionUtils::GetHeroDirectionFromAngle(DirectionMap, angle);
        }
    }

    void InputComponent::PopulateSpecificWidgets()
    {
        ComponentBase::PopulateSpecificWidgets();
        IsGameObjectUISpecified = true;

        //Modify Direction map as table 
        if (ImGui::TreeNode("DirectionMap"))
        {
            if (ImGui::BeginTable("split2", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
            {
                //       |unmodified  key|           old           new         value
                std::map<std::pair<int, int>, std::pair<std::pair<int, int>, Direction>> keysToUpdate;

                //First default two strings to indicate the categories
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Range");

                ImGui::TableNextColumn();
                ImGui::Text("Direction");

                for (const auto& [key, value] : DirectionMap)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    //Make a copy of the current key that we can modify
                    std::pair<int, int> editedKey = key;
                    bool changed = false;

                    ImGui::PushID(key.first);
                    ImGui::PushID(key.second);

                    // Set a fixed width for both input fields
                    const float availWidth = ImGui::GetContentRegionAvail().x;
                    const float spacing = ImGui::GetStyle().ItemSpacing.x;
                    const float inputWidth = (availWidth - spacing) / 2.0f;

                    // First input field
                    ImGui::SetNextItemWidth(inputWidth);
                    const bool valueChanged = ImGui::InputInt("##Key", &editedKey.first, 0, 0);
                    const bool lostFocusAfterEdit = ImGui::IsItemDeactivatedAfterEdit();

                    //If range is not between 0,360, revert
                    if (!Math::IsInRange(editedKey.first, 0, 360))
                    {
                        editedKey.first = std::max(0, std::min(360, editedKey.first));
                    }

                    // Second input field
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(inputWidth);
                    const bool valueChanged2 = ImGui::InputInt("##Key2", &editedKey.second, 0, 0);
                    const bool lostFocusAfterEdit2 = ImGui::IsItemDeactivatedAfterEdit();

                    //If range is not between 0,360, revert
                    if (!Math::IsInRange(editedKey.second, 0, 360))
                    {
                        editedKey.second = std::max(0, std::min(360, editedKey.second));
                    }

                    if ((valueChanged && lostFocusAfterEdit) || (valueChanged2 && lostFocusAfterEdit2))
                        changed = true;

                    if (changed)
                        keysToUpdate[key] = std::pair<std::pair<int, int>, Direction>{editedKey, value};

                    ImGui::TableNextColumn();
                    ImGui::Text(EnumToString(value));

                    ImGui::PopID();
                    ImGui::PopID();
                }

                //If there are any change, remove from DirectionMap and add it back again 
                for (const auto& [oldKey, newKeyAndValue] : keysToUpdate)
                {
                    //remove old entry
                    DirectionMap.erase(oldKey);

                    //Add new entry
                    DirectionMap[newKeyAndValue.first] = newKeyAndValue.second;
                }
            }
            ImGui::EndTable();

            ImGui::TreePop();
        }
    }
}
