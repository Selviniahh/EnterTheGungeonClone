#pragma once
#include "../../Core/GameObjectBase.h"
#include <SFML/Graphics/Texture.hpp>
#include <vector>

namespace ETG
{
    class GunBase;

    class AmmoBarUI : public GameObjectBase
    {
    private:
        // Textures
        std::shared_ptr<sf::Texture> ammoBarTexture;
        std::shared_ptr<sf::Texture> ammoDisplayTexture;

        // Draw properties
        std::vector<DrawProperties> ammoDrawProps;

        // Positions
        std::vector<sf::Vector2f> ammoDisplayPositions;

        // References
        GunBase* currentGun = nullptr;

    public:
        AmmoBarUI();
        ~AmmoBarUI() override = default;

        void Initialize() override;
        void Update() override;
        void Draw() override;

        // Set the gun to display ammo for
        void SetGun(GunBase* gun) { currentGun = gun; }

        // Update the ammo display based on current gun state
        void FlipTexture(bool horizontally = true, bool vertically = true);

        BOOST_DESCRIBE_CLASS(AmmoBarUI, (GameObjectBase),(),(ammoBarTexture, ammoDisplayTexture),(currentGun))
    };
}
