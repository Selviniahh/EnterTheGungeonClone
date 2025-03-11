#pragma once
#include <memory>
#include <imgui.h>
#include "../ComponentBase.h"
#include "../../Animation/AnimationManager.h"
#include "../../Utils/Interface/IAnimationComponent.h"
#include "../../Engine/UI/UIUtils.h"
#include "../../Utils/StrManipulateUtil.h"

namespace ETG
{
    enum class HeroStateEnum;

    template <typename StateEnum>
    class BaseAnimComp : public ComponentBase, public IAnimationComponent
    {
    public:
        // Override the base class Initialize method to register with owner
        void Initialize() override
        {
            GameObjectBase::Initialize();

            if (!Owner) throw std::runtime_error("Owner cannot be empty. Every animation should be an owner game object.");
            
            // Register with owner
            Owner->SetAnimationInterface(this);
        }

        virtual void SetAnimations();
        void Update(const StateEnum& stateEnum, const AnimationKey& animKey);
        virtual void Draw(const sf::Vector2f& position);
        virtual void Draw(sf::Vector2f position, sf::Vector2f Origin, sf::Vector2f Scale, float Rotation, float depth);
        void PopulateSpecificWidgets() override;

        // Implement IAnimationComponent interface
        [[nodiscard]] sf::IntRect GetCurrentTextureRect() const override { return CurrTexRect; }

        //TODO: This GetOrigin and all relative stuffs needs to go from here 
        [[nodiscard]] sf::Vector2f GetOrigin() const override { return RelativeOrigin; }

        sf::IntRect CurrTexRect;
        sf::Vector2f RelativeOrigin{0.f, 0.f};
        std::shared_ptr<sf::Texture> CurrentTex;

        //Animation properties
        std::unordered_map<StateEnum, AnimationManager> AnimManagerDict{};
        StateEnum CurrentState;
        AnimationKey CurrentAnimStateKey;


        BOOST_DESCRIBE_CLASS(BaseAnimComp, (ComponentBase), (CurrentTex, CurrTexRect, RelativeOrigin), (), ())
    };

    //-------------------------------------------------------------Definition-------------------------------------------------------------

    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::Update(const StateEnum& stateEnum, const AnimationKey& animKey)
    {
        CurrentState = stateEnum;
        CurrentAnimStateKey = animKey;

        auto& animManager = AnimManagerDict[CurrentState];
        animManager.Update(CurrentAnimStateKey);

        const auto& animState = animManager.AnimationDict[CurrentAnimStateKey];
        CurrTexRect = animState.CurrRect;
        CurrentTex = animState.GetCurrentFrameAsTexture();
        Owner->Texture = CurrentTex;
        RelativeOrigin = animManager.AnimationDict[AnimManagerDict[CurrentState].LastKey].Origin;
    }

    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::Draw(const sf::Vector2f& position)
    {
        if (!AnimManagerDict.contains(CurrentState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");

        AnimManagerDict[CurrentState].Draw(CurrentTex, position, sf::Color::White, this->Rotation, RelativeOrigin, this->Scale, this->Depth);
    }

    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::Draw(const sf::Vector2f position, const sf::Vector2f Origin, const sf::Vector2f Scale, const float Rotation, const float depth)
    {
        if (!AnimManagerDict.contains(CurrentState)) throw std::runtime_error("AnimManagerDict doesn't contain given state");

        AnimManagerDict[CurrentState].Draw(CurrentTex, position, sf::Color::White, Rotation, Origin, Scale, depth);
    }

    template <typename StateEnum>
    void BaseAnimComp<StateEnum>::SetAnimations()
    {
    }

    //-----------------------------------------UI----------------------------------------
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
