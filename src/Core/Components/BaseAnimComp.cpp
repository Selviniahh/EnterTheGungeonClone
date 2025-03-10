#include "BaseAnimComp.h"
#include <imgui.h>

#include "../../Engine/UI/UIUtils.h"
#include "../../Utils/StrManipulateUtil.h"

template void ETG::BaseAnimComp<ETG::HeroStateEnum>::PopulateSpecificWidgets();
template void ETG::BaseAnimComp<ETG::GunStateEnum>::PopulateSpecificWidgets();

namespace ETG
{
    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::PopulateSpecificWidgets()
    {
        ComponentBase::PopulateSpecificWidgets();

        // Display CurrentState
        UIUtils::BeginProperty("Current State");
        ImGui::Text("%s", EnumToString<StateEnum>(CurrentState));
        UIUtils::EndProperty();

        // Display CurrentAnimStateKey
        UIUtils::BeginProperty("Current Animation Key");
        UIUtils::DisplayAnimationKey(CurrentAnimStateKey);
        UIUtils::EndProperty();

        // Display CurrentTex (texture preview)
        UIUtils::BeginProperty("Current Texture");
        UIUtils::DisplayTexture(CurrentTex);
        UIUtils::EndProperty();

        // Display CurrTexRect
        if (ImGui::TreeNode("Texture Rectangle##CurrTexRect"))
        {
            UIUtils::DisplayIntRectangle(CurrTexRect);
            ImGui::TreePop();
        }

        // Display RelativeOrigin
        UIUtils::BeginProperty("Relative Origin");
        ImGui::InputFloat2("##RelOrigin", &RelativeOrigin.x);
        UIUtils::EndProperty();

        // Display AnimManagerDict
        if (ImGui::TreeNode("Animation Managers Dictionary"))
        {
            ImGui::Text("Size: %zu animation managers", AnimManagerDict.size());
            ImGui::Separator();

            // Iterate through AnimManagerDict and display each entry
            int index = 0;
            for (auto& [stateEnum, animManager] : AnimManagerDict)
            {
                ImGui::PushID(index++);

                std::string FirstStr = EnumToString(stateEnum);
                std::string stateLabel = "State: " + FirstStr;
                bool isCurrentState = (stateEnum == CurrentState);

                // Highlight current state
                if (isCurrentState) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow for current state

                if (ImGui::TreeNode(stateLabel.c_str()))
                {
                    // Current state indicator
                    UIUtils::BeginProperty("Is Current State");
                    ImGui::BeginDisabled(true);
                    ImGui::Checkbox("##IsCurrent", &isCurrentState);
                    ImGui::EndDisabled();
                    UIUtils::EndProperty();

                    // Display AnimationManager for this state
                    UIUtils::DisplayAnimationManager("Animation Manager", animManager);

                    ImGui::TreePop();
                }

                if (isCurrentState)
                {
                    ImGui::PopStyleColor();
                }

                ImGui::PopID();
            }

            ImGui::TreePop();
        }
    }
}
