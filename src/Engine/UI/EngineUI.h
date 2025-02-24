#pragma once

class GameObjectBase;

class EngineUI
{
    friend void ImGuiSetRelativeOrientation(GameObjectBase* obj);
    friend void ImGuiSetAbsoluteOrientation(GameObjectBase* obj);
};
