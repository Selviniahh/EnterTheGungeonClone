#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "../Core/GameObjectBase.h"
#include "../Items//ActiveItem.h"
#include "../Items/PassiveItem.h"

namespace ETG
{
    class GunBase;
    class Hero;

    class UserInterface : public GameObjectBase
    {
    private:
        sf::Texture Frame;
        sf::Texture AmmoBar;
        sf::Texture AmmoDisplay;
        GunBase* Gun;
        sf::Vector2f GunPosition;
        sf::Vector2f AmmoBarPosition;
        std::vector<sf::Vector2i> AmmoArr{};
        int lastAmmoCount = 8;
        bool IsReloaded = true;
        bool RemoveLast = true;
        Hero* Hero = nullptr;
        
        sf::Vector2f FrameOffsetPerc{4,3.5};
        float AmmoBarOffsetPercX = 2.f;

        sf::Vector2f GameScreenSize;

    public:
        UserInterface();
        void Initialize() override;
        void Update() override;
        void Draw() override;

        void SetActiveItem(ActiveItem* item);
        void SetPassiveItem(PassiveItem* item);

    private:
        ActiveItem* m_ActiveItem = nullptr;
        PassiveItem* m_PassiveItem = nullptr;

        sf::RectangleShape ActiveItemUI;
        sf::RectangleShape PassiveItemIcon;
        void DrawActiveItemUI();
        void DrawPassiveItemUI();
        BOOST_DESCRIBE_CLASS(UserInterface,(GameObjectBase),
            (IsReloaded),
            (),
            ()
            )
    };

}
