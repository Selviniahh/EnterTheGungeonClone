#include "EngineUI.h"
#include "../../Managers/GameManager.h"
#include <imgui-SFML.h>
#include <imgui.h>

void EngineUI::Initialize()
{
    //Initialize Imgui-SFML after creating the window
    if (!ImGui::SFML::Init(*ETG::Window)) throw std::runtime_error("Cannot initialize ImGUI with the given Window");

    ETG::GameState::GetInstance().SetEngineUISize(windowSize);
    windowSize = {300, (float)(ETG::Window->getSize().y)};

    LoadFont();
}

void EngineUI::Update() const
{
    //Update ImGUI-SFML with the frame delta time
    ImGui::SFML::Update(*ETG::Window, ETG::ElapsedTimeClock);

    // Begin a new ImGui window docked to the right
    ImGui::SetNextWindowPos(ImVec2((float)ETG::Window->getSize().x - windowSize.x, 0));
    ImGui::SetNextWindowSize(ImVec2(windowSize));

    ImGui::Begin("Details Pane", nullptr);
    UpdateDetailsPanel(ETG::GameState::GetInstance().GetSceneObj());

    //The end
    ImGui::End();
}

void EngineUI::Draw()
{
    //Render
    ImGui::SFML::Render(*ETG::Globals::Window);
}

void EngineUI::UpdateDetailsPanel(const std::vector<GameObject*>& SceneObjects)
{
    static int SelectedIdx = 0;

    //NOTE: Open the Game Objects pane by default
    ImGui::SetNextItemOpen(true,ImGuiCond_Once);
    
    //List all objects
    if (ImGui::CollapsingHeader("Game Objects", ImGuiTreeNodeFlags_None))
    {
        for (int i = 0; i < SceneObjects.size(); ++i)
        {
            ImGui::PushID(i);

            //The boolean only for highlighting selection
            const bool isSelected = (SelectedIdx == i);
            
            if (ImGui::Selectable(SceneObjects[i]->GetObjectName().c_str(), isSelected))
                SelectedIdx = i;

            ImGui::PopID();
        }
    }

    //NOTE: Open the details pane by default
    ImGui::SetNextItemOpen(true,ImGuiCond_Once);

    //Show details of selected objects
    if (ImGui::CollapsingHeader("Details", ImGuiTreeNodeFlags_None))
    {
        //Open the pane by default once
        ImGui::SetNextItemOpen(true,ImGuiCond_Once);
        
        if (SelectedIdx >= 0 && SelectedIdx < SceneObjects.size())
        {
            if(ImGui::TreeNode("Absolute Orientation"))
            {
                SceneObjects[SelectedIdx]->ImGuiSetAbsoluteOrientation();
                ImGui::TreePop();   
            }

            if(ImGui::TreeNode("Relative Orientation"))
            {
                SceneObjects[SelectedIdx]->ImGuiSetRelativeOrientation();
                ImGui::TreePop();   
            }
        }
    }
}

void EngineUI::LoadFont()
{
    const ImGuiIO io = ImGui::GetIO();
    io.Fonts->Clear();

    const std::filesystem::path FontPath = std::filesystem::path(RESOURCE_PATH) / "Fonts" / "SegoeUI.ttf";
    SegoeFont = io.Fonts->AddFontFromFileTTF(FontPath.generic_string().data(), 18.f);
    if (SegoeFont == nullptr) throw std::runtime_error("Failed to load font from " + FontPath.generic_string());
    if (!ImGui::SFML::UpdateFontTexture()) throw std::runtime_error("Font cannot be load.");
}
