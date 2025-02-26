#include <iostream>
#include <imgui-SFML.h>
#include <imgui.h>
#include "Engine.h"
#include "../Managers/GameManager.h"
#include "../Managers/InputManager.h"

bool Engine::CurrentGameFocus = false;
bool Engine::PreviousGameFocus = false;

using namespace ETG::Globals;
using namespace ETG;

void Engine::Initialize()
{
    //Initialize Imgui-SFML after creating the window
    if (!ImGui::SFML::Init(*Window)) throw std::runtime_error("Cannot initialize ImGUI with the given Window");

    GameState::GetInstance().SetEngineUISize(windowSize);
    windowSize = {300, (float)(Window->getSize().y)};
    std::cout << std::unitbuf;

    LoadFont();
}

void Engine::Update() const
{
    //Update ImGUI-SFML with the frame delta time
    ImGui::SFML::Update(*Window, ElapsedTimeClock);

    // Begin a new ImGui window docked to the right
    ImGui::SetNextWindowPos(ImVec2((float)Window->getSize().x - windowSize.x, 0));
    ImGui::SetNextWindowSize(ImVec2(windowSize));

    ImGui::Begin("Details Pane", nullptr);
    UpdateDetailsPanel(ETG::GameState::GetInstance().GetSceneObj());

    PreviousGameFocus = CurrentGameFocus;
    CurrentGameFocus = !(ImGui::IsWindowHovered() || ImGui::IsWindowFocused());

    //The end
    ImGui::End();
}

bool Engine::IsGameWindowFocused()
{
    // Check if the game has focus; if not, ignore input.
    if (!CurrentGameFocus) return false;

    // Determine if focus was just gained (Current is true, Previous was false)
    if (CurrentGameFocus && !Engine::PreviousGameFocus)
        InputManager::LeftClickRequired = true;

    // Process events to check for mouse release
    if (InputManager::LeftClickRequired && GameManager::GameEvent.type == sf::Event::MouseButtonReleased && ETG::GameManager::GameEvent.mouseButton.button == sf::Mouse::Left)
    {
        InputManager::LeftClickRequired = false;
    }
    
    // If LeftClickRequired is still true, wait for release before processing input
    if (InputManager::LeftClickRequired)
    {
        return false;
    }
    return true;
}

void Engine::Draw()
{
    //Render
    ImGui::SFML::Render(*Window);
}

//Probably the way I am making selection is wrong. Fix it with the convenient way ImGUI handled before 
void Engine::UpdateDetailsPanel(const std::unordered_map<std::string,GameObject*>& SceneObjects)
{
    static GameObject* SelectedObj = nullptr;
    static bool isSelected = false;
    
    //NOTE: Open the Game Objects pane by default
    ImGui::SetNextItemOpen(true,ImGuiCond_Once);
    
    //List all objects and determine the Selected Object
    if (ImGui::CollapsingHeader("Game Objects", ImGuiTreeNodeFlags_None))
    {
        for (const auto& pair : SceneObjects)
        {
            ImGui::PushID(pair.first.c_str());

            if (ImGui::Selectable(pair.second->GetObjectName().c_str(), isSelected)) //If the name selected through GUI, isSelected will be set to true 
            {
                SelectedObj = pair.second;
            }

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
        
        if (SelectedObj != nullptr)
        {
            if(ImGui::TreeNode("Absolute Orientation"))
            {
                ImGuiSetAbsoluteOrientation(SelectedObj);
                ImGui::TreePop();   
            }

            if(ImGui::TreeNode("Relative Orientation"))
            {
                ImGuiSetRelativeOrientation(SelectedObj);
                ImGui::TreePop();   
            }
        }
    }
}

void Engine::LoadFont()
{
    const ImGuiIO io = ImGui::GetIO();
    io.Fonts->Clear();

    const std::filesystem::path FontPath = std::filesystem::path(RESOURCE_PATH) / "Fonts" / "SegoeUI.ttf";
    SegoeFont = io.Fonts->AddFontFromFileTTF(FontPath.generic_string().data(), 18.f);
    if (SegoeFont == nullptr) throw std::runtime_error("Failed to load font from " + FontPath.generic_string());
    if (!ImGui::SFML::UpdateFontTexture()) throw std::runtime_error("Font cannot be load.");
}
