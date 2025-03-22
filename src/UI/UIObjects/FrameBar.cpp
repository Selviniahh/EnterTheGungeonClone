#include "FrameBar.h"
#include "../../Guns/Base/GunBase.h"
#include "../../Managers/SpriteBatch.h"

namespace ETG
{
    FrameBar::FrameBar(const std::string& texturePath, const BarType type) : barType(type)
    {
        Texture = std::make_shared<sf::Texture>();

        if (!Texture->loadFromFile(texturePath))
            throw std::runtime_error("Failed to load Frame texture from: " + texturePath);

        Origin = {Texture->getSize().x / 2.f, Texture->getSize().y / 2.f};
        FrameBar::Initialize();
    }

    void FrameBar::Initialize()
    {
        GameObjectBase::Initialize();
    }

    void FrameBar::Update()
    {
        GameObjectBase::Update();
        
        // Update content draw properties based on content type
        if (barType == BarType::GunBar && gunContent)
        {
            contentDrawProps.Texture = gunContent->Texture.get();
            SetDrawPropsOrientation();
        }
        else if (barType == BarType::ActiveItemBar && itemContent)
        {
            contentDrawProps.Texture = itemContent->Texture.get();
            SetDrawPropsOrientation();
        }
    }

    void FrameBar::Draw()
    {
        // First draw the frame
        GameObjectBase::Draw();
        
        // Then draw the content if available
        if ((barType == BarType::GunBar && gunContent && gunContent->Texture) ||
            (barType == BarType::ActiveItemBar && itemContent && itemContent->Texture))
        {
            SpriteBatch::Draw(contentDrawProps);
        }
    }

    void FrameBar::SetDrawPropsOrientation()
    {
        contentDrawProps.Position = Position;  // Center on frame position
        contentDrawProps.Scale = {contentScale, contentScale};
        contentDrawProps.Origin = {
            static_cast<float>(contentDrawProps.Texture->getSize().x) / 2.0f,
            static_cast<float>(contentDrawProps.Texture->getSize().y) / 2.0f
        };
    }

    void FrameBar::SetGun(GunBase* gun)
    {
        gunContent = gun;
        // Reset the other content type
        itemContent = nullptr;
        
        // If bar type doesn't match the content, update it
        if (barType != BarType::GunBar)
            barType = BarType::GunBar;
    }

    void FrameBar::SetActiveItem(GameObjectBase* item)
    {
        itemContent = item;
        // Reset the other content type
        gunContent = nullptr;
        
        // If bar type doesn't match the content, update it
        if (barType != BarType::ActiveItemBar)
            barType = BarType::ActiveItemBar;
    }
}