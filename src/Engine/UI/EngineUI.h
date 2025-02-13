#pragma once
#include <vector>

class GameObject;
struct ImFont;


class EngineUI
{
public:
    void LoadFont();
    void Initialize();
    void Update();
    void Draw();

    void UpdateDetailsPanel(const std::vector<GameObject*>& sceneObjects);

    ImFont* SegoeFont;
};
