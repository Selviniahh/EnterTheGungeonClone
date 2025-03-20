#pragma once
#include "../../Core/GameObjectBase.h"

namespace ETG
{
    class FrameBar : public GameObjectBase
    {
    public:
        explicit FrameBar(const std::string& texturePath);
        ~FrameBar() override = default;
        void Initialize() override;
        void Draw() override;
        void Update() override;
    };

    inline FrameBar::FrameBar(const std::string& texturePath)
    {
        Texture = std::make_shared<sf::Texture>();

        if (!Texture->loadFromFile(texturePath))
            throw std::runtime_error("Failed to load Frame.png");

        Origin = {Texture->getSize().x / 2.f, Texture->getSize().y / 2.f};

        FrameBar::Initialize();
    }

    inline void FrameBar::Initialize()
    {
        GameObjectBase::Initialize();
    }

    inline void FrameBar::Draw()
    {
        GameObjectBase::Draw();
    }

    inline void FrameBar::Update()
    {
        GameObjectBase::Update();
    }
}
