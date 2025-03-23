#include <filesystem>
#include "FrameLeftProgressBar.h"
#include "../../Items/Active/ActiveItemBase.h"
#include "../../Managers/Globals.h"
#include "../../Managers/GameState.h"
#include "../../Managers/SpriteBatch.h"
#include "../../Utils/Math.h"

ETG::FrameLeftProgressBar::FrameLeftProgressBar()
{
    Texture = std::make_shared<sf::Texture>();
    if (!Texture->loadFromFile((std::filesystem::path(RESOURCE_PATH) / "UI" / "FrameLeftProgressBar.png").generic_string()))
        throw std::runtime_error("Failed to load FrameLeftProgressBar texture");

    // Set up the progress rectangle
    progressRect.setFillColor(progressColor);

    // Set dimensions based on texture
    if (Texture)
    {
        maxWidth = static_cast<float>(Texture->getSize().x - 4); // Slight margin
        maxHeight = static_cast<float>(Texture->getSize().y - 4);
        progressRect.setSize({maxWidth, maxHeight}); // Start with full height
    }

    FrameLeftProgressBar::Initialize();
}

void ETG::FrameLeftProgressBar::Initialize()
{
    GameObjectBase::Initialize();

    Origin = {Texture->getSize().x / 2.f, Texture->getSize().y / 2.f};
}

void ETG::FrameLeftProgressBar::Update()
{
    GameObjectBase::Update();

    //Calculate Top and Bottom center. Bcs progressRect is not GameObjectBase derived, we have to calculate center origin ourselves
    ProgTopCenter = {Position.x, Position.y - maxHeight / 2};
    ProgBottomCenter = {Position.x, Position.y + maxHeight / 2};

    if (activeItem)
        
        //ACTIVE STATE: Progress decreases from top (100%) to bottom (0%)
        if (activeItem->ActiveItemState == ActiveItemState::Consuming)
        {
            //Set active item color back to normal 
            activeItem->SetColor(sf::Color::White);
            progressRect.setFillColor(sf::Color::White);
            progressRect.setPosition(ProgTopCenter);
            progressRect.setSize({maxWidth, CurrProgressY});
            progressRect.setOrigin(progressRect.getSize().x / 2, 0);
            CurrProgressY = Math::IntervalLerp(0.f, ProgBottomCenter.y - ProgTopCenter.y, activeItem->TotalConsumeTime, activeItem->ConsumeTimer);
        }

        //Cooldown is over. Item is ready to be consumed. Set color back to normal.
        else if (activeItem->ActiveItemState == ActiveItemState::Ready)
        {
            //Set item to 
            activeItem->SetColor(sf::Color::White);
        }

        // COOLDOWN STATE: Progress increases from bottom (0%) to top (100%)
        else if (activeItem->ActiveItemState == ActiveItemState::Cooldown)
        {
            std::cout << "Cooldown state: " << std::endl;
            activeItem->SetColor(sf::Color{94,94,94});

        }
}


void ETG::FrameLeftProgressBar::Draw()
{
    // Draw the frame background first
    GameObjectBase::Draw();

    // Draw rectangle with higher depth to ensure it appears on top
    sf::RenderWindow* window = GameState::GetInstance().GetRenderWindow();
    if (window)
    {
        // Fallback direct drawing in case SpriteBatch method fails
        window->draw(progressRect);


        // Try SpriteBatch method as well
        SpriteBatch::AddDebugCircle(ProgTopCenter, 15);
        SpriteBatch::AddDebugCircle(ProgBottomCenter, 15);
    }
}
