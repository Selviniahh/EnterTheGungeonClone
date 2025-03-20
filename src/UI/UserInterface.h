#pragma once
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include "../Core/GameObjectBase.h"
#include "UIObjects/AmmoBarUI.h"
#include "UIObjects/AmmoCounter.h"
#include "UIObjects/AmmoIndicatorsUI.h"
#include "UIObjects/FrameBar.h"

namespace ETG
{
    class GunBase;
    class Hero;
    class FrameBar;
    class ReloadSlider;

    class UserInterface : public GameObjectBase
    {
    private:
        // Textures
        // std::shared_ptr<sf::Texture> frameTexture;

        // Draw properties
        // DrawProperties RightFrameDrawProps;
        DrawProperties gunDrawProps;

        // Positions
        // sf::Vector2f RightFramePosition;

        // References
        GunBase* CurrentGun = nullptr;
        Hero* hero = nullptr;

        // UI Components
        std::unique_ptr<FrameBar> RightGunFrame;
        std::unique_ptr<FrameBar> LeftActiveItemFrame;
        std::unique_ptr<AmmoBarUI> ammoBarBottom;
        std::unique_ptr<AmmoBarUI> ammoBarTop;
        std::unique_ptr<AmmoIndicatorsUI> ammoIndicators;
        std::unique_ptr<AmmoCounter> ammoCounter;

        // Configuration
        sf::Vector2f FrameOffsetPerc{4, 3.5};
        // sf::Vector2u RightFrameSize;

        //Ammo display at right UI
        float AmmoBarOffsetPercX = 2.f;
        float InitialAmmoBarOffsetY = 50.f;

        //Left passive item UI
        float LeftXOffsetPerItem = 10.f;
        float InitialLeftOffsetX = 10.f;
        float InitialLeftOffsetY = 30.f;

        //Get WindowSize - EngineUISize to calculate GameScreen
        sf::Vector2f GameScreenSize;

    public:
        UserInterface();
        void InitializeFrameProperties();
        void InitializeAmmoBar();
        void Initialize() override;
        void UpdateGunUIProperties();
        void Update() override;
        void DrawEquippedPassiveItemsAtLeftUI() const;
        void Draw() override;

        BOOST_DESCRIBE_CLASS(UserInterface, (GameObjectBase),
                             (), (), (CurrentGun, FrameOffsetPerc, AmmoBarOffsetPercX, InitialAmmoBarOffsetY, GameScreenSize))
    };
}
