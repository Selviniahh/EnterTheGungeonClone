#pragma once

#include <imgui.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class GameObject
{
    struct DrawProperties
    {
        sf::Vector2f Position{0, 0};
        sf::Vector2f Scale{0, 0};
        sf::Vector2f Origin{0, 0};
        float Rotation{};
        float Depth{};
    };

public:
    GameObject() = default;
    virtual ~GameObject() = default;

protected:
    virtual void Initialize() {}
    virtual void Draw() {}

    virtual void Update()
    {
        //Calculate the final drawing properties. The Base position modified from the source and relative pos given in the UI will be summed to form the final value before being drawn
        ComputeDrawProperties();
    }

    std::string ObjectName{"Default"};
    sf::Texture Texture;

    //Base position of GameObjects
    //Inherited Objects such as Gun's position will be attached to hand pos in tick. After the object manipulations are completed, the relative offsets needs given in UI needs to be applied
    //and result will be stored in FinalPos, FinalRot etc. Final properties will be drawn.    
    sf::Vector2f Position{0, 0};
    sf::Vector2f Scale = {1, 1};
    float Rotation{};
    sf::Vector2f Origin{0.f, 0.f};
    float Depth{};

private:
    //Relative Offsets for GameObjects.
    sf::Vector2f RelativePos{0.f, 0.f};
    sf::Vector2f RelativeScale = {1, 1};
    float RelativeRotation{};
    sf::Vector2f RelativeOrigin{0.f, 0.f};

    //Previous Relative Offsets
    sf::Vector2f PrevRelativePos{0.f, 0.f};
    sf::Vector2f PrevRelativeScale{0.f, 0.f};
    sf::Vector2f PrevRelativeRot{0.f, 0.f};

    //Contains the final drawing properties. 
    DrawProperties DrawProps;

public:
    // Only the drawing code (or renderer) is expected to use these values.
    [[nodiscard]] const DrawProperties& GetDrawProperties() const {return DrawProps;}
    
    virtual std::string& GetObjectName() { return ObjectName; }
    [[nodiscard]] const sf::Vector2f& GetPosition() const { return Position; }
    [[nodiscard]] const sf::Vector2f& GetScale() const { return Scale; }
    [[nodiscard]] const sf::Vector2f& GetOrigin() const { return Origin; }

    void SetObjectName(const std::string& ObjectName) { this->ObjectName = ObjectName; }
    void SetPosition(const sf::Vector2f& Position) { this->Position = Position; }
    void SetScale(const sf::Vector2f& Scale) { this->Scale = Scale; }
    void SetOrigin(const sf::Vector2f& Origin) { this->Origin = Origin; }

    void ComputeDrawProperties()
    {
        DrawProps.Position = Position + RelativePos;
        DrawProps.Scale = {Scale.x * RelativeScale.x, Scale.y * RelativeScale.y};
        DrawProps.Rotation = Rotation + RelativeRotation;
        DrawProps.Origin = Origin + RelativeOrigin;
    }
    
    // When the UI changes, recalc the final position from BasePosition.
    virtual void ImGuiSetDefaultValues()
    {
        //NOTE: Position
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Position");
        ImGui::SameLine();
        if (ImGui::InputFloat2("##PosX", &RelativePos.x, "%.3f"))
        {
            //Revert what I've done last time and then apply RelativePos again
            const sf::Vector2f delta = {RelativePos.x - PrevRelativePos.x, RelativePos.y - PrevRelativePos.y};
            DrawProps.Position += delta;
            PrevRelativePos = RelativePos;
        }

        //NOTE: Scale
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Scale");
        ImGui::SameLine();
        if (ImGui::InputFloat2("##Scale", &RelativeScale.x, "%.3f"))
        {
            const sf::Vector2f delta{RelativeScale.x - PrevRelativeScale.x, RelativeScale.y - PrevRelativeScale.y};
            DrawProps.Scale += delta;
            PrevRelativeScale = RelativeScale;
        }

        //NOTE: Rotation
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Rotation");
        ImGui::SameLine();
        ImGui::SliderFloat("##Rot", &RelativeRotation, 0.0f, 360.0f);
    }
};
