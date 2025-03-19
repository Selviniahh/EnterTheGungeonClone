// Add these function declarations to your UIUtils.h
#pragma once

class Animation;
class AnimationManager;

#include <memory>
#include <variant>
#include "../../Animation/Animation.h"
#include "../../Animation/AnimationManager.h"

namespace sf
{
    class Texture;
    template<typename T> struct Rect;
    typedef Rect<int> IntRect;
}

namespace UIUtils
{
    void DisplayIntRectangle(sf::IntRect& rect);
    void DisplayAnimation(const char* label, Animation& value);
    void DisplayTexture(const std::shared_ptr<sf::Texture>& value);
    void DisplayAnimationKey(const AnimationKey& key);
    void DisplayAnimationManager(const char* label, AnimationManager& manager);
    void DisplayColorPicker(const char* label, sf::Color& color);

    void BeginProperty(const char* label);
    void EndProperty();
}