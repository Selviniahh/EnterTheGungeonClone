#include <iostream>
#include <imgui-SFML.h>
#include <imgui.h>
#include "Engine.h"
#include "../Characters/Hero.h"
#include "../Managers/GameManager.h"
#include "../Managers/InputManager.h"
#include "../Managers/TypeRegistry.h"
#include "../Utils/Math.h"
#include "UI/EngineUI.h"

bool Engine::CurrentGameFocus = false;
bool Engine::PreviousGameFocus = false;

bool Engine::AbsoluteOrientationOpen = false;
bool Engine::RelativeOrientationOpen = false;
bool Engine::PropertiesOpen = false;

using namespace ETG::Globals;
using namespace ETG;

void Engine::Initialize()
{
    //Initialize Imgui-SFML after creating the window
    if (!ImGui::SFML::Init(*Window)) throw std::runtime_error("Cannot initialize ImGUI with the given Window");

    GameState::GetInstance().SetEngineUISize(&windowSize);
    windowSize = {400, (float)(Window->getSize().y)};
    std::cout << std::unitbuf;

    LoadFont();
    TypeRegistry::InitializeTypeRegistry();
}

void Engine::Update()
{
    //Update ImGUI-SFML with the frame delta time
    ImGui::SFML::Update(*Window, ElapsedTimeClock);

    // Begin a new ImGui window docked to the right
    // Only set position and size when first creating the window
    ImGui::SetNextWindowPos(ImVec2((float)Window->getSize().x - windowSize.x, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(windowSize), ImGuiCond_FirstUseEver);

    // Use no flags to allow all default window behaviors (dragging, resizing)
    ImGui::Begin("Details Pane", nullptr);

    //If window is updated, need to assign to this variable so Game UI can be updated
    windowSize = ImGui::GetWindowSize();
    
    UpdateDetailsPanel();

    PreviousGameFocus = CurrentGameFocus;
    CurrentGameFocus = !(ImGui::IsWindowHovered() || ImGui::IsWindowFocused());

    //The end
    ImGui::End();
}

bool Engine::IsGameWindowFocused()
{
    // First check if ImGui wants to capture mouse input
    const ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return false;

    // Check if the game has focus; if not, ignore input.
    if (!CurrentGameFocus) 
        return false;

    // Determine if focus was just gained
    if (CurrentGameFocus && !Engine::PreviousGameFocus)
        InputManager::LeftClickRequired = true;

    // Process events to check for mouse release
    if (InputManager::LeftClickRequired && GameManager::GameEvent.type == sf::Event::MouseButtonReleased && 
        ETG::GameManager::GameEvent.mouseButton.button == sf::Mouse::Left)
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

void Engine::UpdateDetailsPanel()
{
    //NOTE: Open the Game Objects pane by default
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    //Game object pane
    if (ImGui::CollapsingHeader("Hierarchy", ImGuiTreeNodeFlags_None))
    {
        const float hierarchyHeight = Math::CalculatePercentageOfValue((float)ScreenSize.y, 25); 
        // constexpr float hierarchyHeight = 300;
        
        // Create a child window with scrolling enabled
        ImGui::BeginChild("HierarchyScrollingRegion", ImVec2(0, hierarchyHeight), true, ImGuiWindowFlags_HorizontalScrollbar);
        
        // Assuming Scene is the root object
        GameObjectBase* sceneObj = GameState::GetInstance().GetSceneObj();
        DisplayHierarchy(sceneObj);
        
        ImGui::EndChild(); // End the scrollable region
    }

    //NOTE: Open the details pane by default
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    //Show details of selected objects
    if (ImGui::CollapsingHeader("Details", ImGuiTreeNodeFlags_None))
    {
        //Open the pane by default once
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        DisplayProperties();
    }
}


void Engine::DisplayHierarchy(GameObjectBase* object)
{
    ImGui::PushID(object);

    //Is current object has any children prepare to make it expandible node tree instead of just selectable. 
    bool currObjHasChildren = false;
    for (const auto& [name, sceneObj] : GameState::GetInstance().GetSceneObjs())
    {
        if (sceneObj->Owner == object)
        {
            currObjHasChildren = true;
            break;
        }
    }

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (object == GameState::GetInstance().GetSceneObj()) flags |= ImGuiTreeNodeFlags_DefaultOpen; //Default expand the scene objects
    if (SelectedObj == object) flags |= ImGuiTreeNodeFlags_Selected;

    //Create tree node if the object has children. If not create single selectable widget 
    bool isOpen;
    currObjHasChildren
        ? isOpen = ImGui::TreeNodeEx(object->GetObjectName().c_str(), flags)
        : isOpen = ImGui::Selectable(object->GetObjectName().c_str(), object == SelectedObj);

    //Handle selection
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        SelectedObj = object;
    }

    //Find all the children of current object and Draw all of them
    if (isOpen)
    {
        for (const auto& sceneObj : GameState::GetInstance().GetOrderedSceneObjs())
        {
            if (sceneObj->Owner == object)
            {
                DisplayHierarchy(sceneObj);
            }
        }

        if (currObjHasChildren) ImGui::TreePop();
    }

    ImGui::PopID();
}

void Engine::DisplayProperties() const
{
    if (SelectedObj != nullptr)
    {
        //Display the Absolute and Relative orientation only if the SelectedObj is not inherited from ComponentBase
        if (!dynamic_cast<ComponentBase*>(SelectedObj))
        {
            if (ImGui::TreeNode("Absolute Orientation"))
            {
                ImGuiSetAbsoluteOrientation(SelectedObj);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Relative Orientation"))
            {
                ImGuiSetRelativeOrientation(SelectedObj);
                ImGui::TreePop();
            }
        }

        //Use stored state for properties tree node
        ImGui::SetNextItemOpen(PropertiesOpen);
        if (ImGui::TreeNode("Properties"))
        {
            PropertiesOpen = true;
            TypeRegistry::ProcessObject(SelectedObj);

            if (SelectedObj->IsGameObjectUISpecified && ImGui::TreeNode("Current Object"))
            {
                SelectedObj->PopulateSpecificWidgets();
                ImGui::TreePop();                
            }
            
            ImGui::TreePop();
        }
        else
            PropertiesOpen = false;
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
