#include "EngineUI.h"
#include "../../Managers/GameManager.h"
#include <imgui-SFML.h>
#include <imgui.h>

void EngineUI::LoadFont()
{
    const ImGuiIO io = ImGui::GetIO();
    io.Fonts->Clear();

    SegoeFont = io.Fonts->AddFontFromFileTTF("C:/Users/Selviniah/Pictures/ETG/Resources/Fonts/SegoeUI.ttf", 18.f);
    if (SegoeFont == nullptr) throw std::runtime_error("Failed to load font from C:/Users/Selviniah/Pictures/ETG/Resources/Fonts/SegoeUI.ttf");
    if (!ImGui::SFML::UpdateFontTexture()) throw std::runtime_error("Font cannot be load.");
}

void EngineUI::Initialize()
{
    //Initialize Imgui-SFML after creating the window
    if (!ImGui::SFML::Init(*ETG::Window)) throw std::runtime_error("Cannot initialize ImGUI with the given Window");

    LoadFont();
}

void EngineUI::Update()
{
    //Update ImGUI-SFML with the frame delta time
    ImGui::SFML::Update(*ETG::Window, ETG::ElapsedTimeClock);

    // Begin a new ImGui window docked to the right
    constexpr float windowWidth = 300.f;
    const sf::Vector2u winSize = ETG::Window->getSize();
    ImGui::SetNextWindowPos(ImVec2(winSize.x - windowWidth, 0));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, static_cast<float>(winSize.y)));
    ImGui::Begin("Right Docked Panel");
    ImGui::Text("Hello, ImGui!");
    ImGui::Button("Look at this pretty button");
    // You can add additional widgets here
    ImGui::End();

    UpdateDetailsPanel(ETG::GameState::GetInstance().GetSceneObj());
}

void EngineUI::Draw()
{
    //Render
    ImGui::SFML::Render(*ETG::Globals::Window);
}

void EngineUI::UpdateDetailsPanel(const std::vector<GameObject*>& sceneObjects)
{
    static int selected = 0;

    //List all objects
    ImGui::Begin("Scene Objects");
    for (int i = 0; i < sceneObjects.size(); ++i)
    {
        const bool isSelected = (selected == i);
        if (ImGui::Selectable(sceneObjects[i]->GetObjectName().c_str(), isSelected))
            selected = i;
    }
    ImGui::End();

    //Show details of selected objects
    ImGui::Begin("Object Details");
    if (selected >= 0 && selected < sceneObjects.size())
    {
        sceneObjects[selected]->ImGuiInspect();
    }

    ImGui::End();
}
