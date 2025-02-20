#pragma once 

#include <imgui.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;

protected:
    virtual void Initialize() {}
    virtual void Update()
    {
        FinalPosition = BasePosition + RelativePos;
    }
    virtual void Draw() {}

    std::string ObjectName{"Default"};
    sf::Texture Texture;

    //Base position of GameObjects
    //Inherited Objects such as Gun's position will be attached to hand pos in tick. After the object manipulations are completed, the relative offsets needs given in UI needs to be applied
    //and result will be stored in FinalPos, FinalRot etc. Final properties will be drawn.    
    sf::Vector2f BasePosition{0,0};
    sf::Vector2f BaseScale = {1,1};
    float BaseRotation{};
    sf::Vector2f BaseOrigin{0.f, 0.f};
    float BaseDepth{};

    //Final properties
    sf::Vector2f FinalPosition{0.f, 0.f};
    sf::Vector2f FinalScale = {1,1};
    float FinalRotation{};
    sf::Vector2f FinalOrigin{0.f, 0.f};
    float FinalDepth{};

private:
    //Relative Offsets for GameObjects.
    sf::Vector2f RelativePos{0.f, 0.f};
    sf::Vector2f RelativeScale = {1,1};
    float RelativeRotation{};

    //Previous Relative Offsets
    sf::Vector2f PreviousRelativePos{0.f,0.f};
    

public:
    [[nodiscard]] sf::Vector2f& GetPosition() {return BasePosition;}
    [[nodiscard]] sf::Vector2f& GetScale() {return BaseScale;}
    [[nodiscard]] sf::Vector2f& GetOrigin() {return BaseOrigin;}

    virtual std::string& GetObjectName() {return ObjectName;}
    void SetObjectName(const std::string& ObjectName) {this->ObjectName = ObjectName;}

    // When the UI changes, recalc the final position from BasePosition.
    virtual void ImGuiSetDefaultValues()
    {
        ImGui::AlignTextToFramePadding(); ImGui::Text("Position"); ImGui::SameLine();
        if(ImGui::InputFloat("##Pos", &RelativePos.x,1.f, 5.0f, "%.3f"))
        {
            //Revert what I've done last time and then apply RelativePos again
            const sf::Vector2f delta = {RelativePos.x - PreviousRelativePos.x, RelativePos.y - PreviousRelativePos.y};
            FinalPosition += delta;
            PreviousRelativePos.x = RelativePos.x;
        }
        ImGui::AlignTextToFramePadding(); ImGui::Text("Scale");      ImGui::SameLine(); ImGui::InputFloat("##Scale", &RelativeScale.x, 1.f, 10.0f, "%.3f");
        ImGui::AlignTextToFramePadding(); ImGui::Text("Rotation");   ImGui::SameLine(); ImGui::SliderFloat("##Rot", &RelativeRotation, 0.0f, 360.0f);
    }
};