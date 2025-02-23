#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "../../Core/GameObject.h"

struct ImFont;

class EngineUI
{
public:
    void LoadFont();
    void Initialize();
    void Update() const;
    static bool IsGameWindowFocused();
    void Draw();

    static void UpdateDetailsPanel(const std::vector<GameObjectBase*>& SceneObjects);
    static bool CurrentGameFocus;
    static bool PreviousGameFocus;

    ImFont* SegoeFont{};
    sf::Vector2f windowSize;
};
