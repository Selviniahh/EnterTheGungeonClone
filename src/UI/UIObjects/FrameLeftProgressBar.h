#pragma once
#include "../../Core/GameObjectBase.h"
#include <SFML/Graphics/RectangleShape.hpp>

namespace ETG
{
    class ActiveItemBase;
    
    class FrameLeftProgressBar : public GameObjectBase
    {
    public:
        FrameLeftProgressBar();
        ~FrameLeftProgressBar() override = default;
        
        void Initialize() override;
        void Draw() override;
        void Update() override;
        
        // Set the active item to display progress for
        void SetActiveItem(ActiveItemBase* item) { activeItem = item; }
        
        // Get background/foreground colors for customization
        void SetProgressColor(const sf::Color& color) { progressColor = color; }
        
    private:
        ActiveItemBase* activeItem = nullptr;
        sf::Color progressColor = sf::Color(255, 255, 0); // Default yellow
        
        // Progress bar elements
        sf::RectangleShape progressRect;
        float maxWidth = 0.0f;
        float maxHeight = 0.0f;

        float CurrProgressY{}; //This will change in tick
        float TotalProgressLength{}; //ProgTopCenter.y - ProgBottomCenter.y For now it's always 120

        sf::Vector2f ProgTopCenter; //Progress bar's top center position
        sf::Vector2f ProgBottomCenter; //Progress bar's bottom center position
    };
}