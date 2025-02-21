#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

class GameObject;
struct ImFont;


class EngineUI
{
public:
    void LoadFont();
    void Initialize();
    void Update() const;
    void Draw();

    static void UpdateDetailsPanel(const std::vector<GameObject*>& SceneObjects);

    ImFont* SegoeFont{};
    sf::Vector2f windowSize;
};
