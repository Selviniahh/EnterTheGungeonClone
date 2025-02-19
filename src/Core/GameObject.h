#pragma once 

#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>
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
        Position.x += PosRelative.x;
    }
    virtual void Draw() {}

    std::string ObjectName{"Default"};
    sf::Texture Texture;
    sf::Vector2f Position{0.f, 0.f};
    sf::Vector2f Scale = {1,1};
    float Rotation{};
    sf::Vector2f Origin{0.f, 0.f};
    float depth{};

public:
    [[nodiscard]] sf::Vector2f& GetPosition() {return Position;}
    [[nodiscard]] sf::Vector2f& GetScale() {return Scale;}
    [[nodiscard]] sf::Vector2f& GetOrigin() {return Origin;}

    virtual std::string& GetObjectName() {return ObjectName;}
    void SetObjectName(const std::string& ObjectName) {this->ObjectName = ObjectName;}
    
    virtual void ImGuiSetDefaultValues()
    {
        ImGui::AlignTextToFramePadding(); ImGui::Text("Position");   ImGui::SameLine();(ImGui::InputFloat("##Pos", &PosRelative.x,1.f, 5.0f, "%.3f"));
        ImGui::AlignTextToFramePadding(); ImGui::Text("Scale");      ImGui::SameLine(); ImGui::InputFloat("##Scale", &ScaleRelative.x, 1.f, 10.0f, "%.3f");
        ImGui::AlignTextToFramePadding(); ImGui::Text("Rotation");   ImGui::SameLine(); ImGui::SliderFloat("##Rot", &RotRelative, 0.0f, 360.0f);
    }

private:
    //Offsets for GameObjects.
    sf::Vector2f PosRelative{0.f, 0.f};
    sf::Vector2f ScaleRelative = {1,1};
    float RotRelative{};
};