#pragma once
#include <unordered_map>
#include <unordered_set>

#include "../Core/GameObject.h"

struct ImFont;

class Engine
{
public:
    void LoadFont();
    void Initialize();
    void Update();
    void Draw();
    static bool IsGameWindowFocused();

    static bool CurrentGameFocus;
    static bool PreviousGameFocus;

private:
    void UpdateDetailsPanel(std::unordered_map<std::string, ETG::GameObject*>& SceneObjects);
    void RenderGameObject(ETG::GameObject* obj);
    friend void ImGuiSetRelativeOrientation(ETG::GameObject* obj);
    friend void ImGuiSetAbsoluteOrientation(ETG::GameObject* obj);
    friend void ImGuiSetRelativeOrientation();
    void DrawGameObject(ETG::GameObject* object);

    ImFont* SegoeFont{};
    sf::Vector2f windowSize;
    ETG::GameObject* SelectedObj = nullptr;

    std::unordered_set<ETG::GameObject*> OwnerObjects;
};
