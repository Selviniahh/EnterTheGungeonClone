#pragma once
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include "../Core/GameObjectBase.h"
#include "UIObjects/AmmoBarUI.h"
#include "UIObjects/AmmoCounter.h"
#include "UIObjects/AmmoIndicatorsUI.h"

namespace ETG
{
    class GunBase;
    class Hero;

    class UserInterface : public GameObjectBase
    {
    private:
        // Textures
        std::shared_ptr<sf::Texture> frameTexture;

        // Draw properties
        DrawProperties frameDrawProps;
        DrawProperties gunDrawProps;

        // Positions
        sf::Vector2f framePosition;

        // References
        GunBase* CurrentGun = nullptr;
        Hero* hero = nullptr;

        // UI Components
        std::unique_ptr<AmmoBarUI> ammoBarBottom;
        std::unique_ptr<AmmoBarUI> ammoBarTop;
        std::unique_ptr<AmmoIndicatorsUI> ammoIndicators;
        std::unique_ptr<AmmoCounter> ammoCounter;

        // Configuration
        sf::Vector2f FrameOffsetPerc{4, 3.5};
        float AmmoBarOffsetPercX = 2.f;
        float InitialAmmoBarOffsetY = 50.f;
        sf::Vector2u frameSize;

        //Get WindowSize - EngineUISize to calculate GameScreen
        sf::Vector2f GameScreenSize;

    public:
        UserInterface();
        void InitializeFrameProperties();
        void InitializeAmmoBar();
        void Initialize() override;
        void UpdateGunUIProperties();
        void Update() override;
        void Draw() override;

        BOOST_DESCRIBE_CLASS(UserInterface, (GameObjectBase),
                             (),
                             (frameTexture),
                             (CurrentGun))
    };
}
