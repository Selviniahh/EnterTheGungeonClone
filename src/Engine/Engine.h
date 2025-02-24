#pragma once
#include <vector>
#include "../Core/GameObject.h"

struct ImFont;

class Engine
{
public:
    void LoadFont();
    void Initialize();
    void Update() const;
    static bool IsGameWindowFocused();
    friend void ImGuiSetRelativeOrientation();
    void Draw();

    static void UpdateDetailsPanel(const std::vector<GameObjectBase*>& SceneObjects);
    friend void ImGuiSetRelativeOrientation(GameObjectBase* obj);
    friend void ImGuiSetAbsoluteOrientation(GameObjectBase* obj);
    static bool CurrentGameFocus;
    static bool PreviousGameFocus;

    ImFont* SegoeFont{};
    sf::Vector2f windowSize;
};
