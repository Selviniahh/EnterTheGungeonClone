// Managers/SpriteBatch.h
#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Core/GameObjectBase.h"

namespace ETG
{
    class SpriteBatch
    {
    public:
        SpriteBatch() = default;

        void begin();

        //NOTE: Sprites added to a batch are transformed based on the active view
        template <typename T>
        void Draw(const T& drawType, float depth);
        void drawRectOutline(const sf::FloatRect& rect, const sf::Color& color, float thickness, float depth);

        void end(sf::RenderWindow& window);

        static void SimpleDraw(const std::shared_ptr<sf::Texture>& tex, const sf::Vector2f& pos, float Rotation = 0, sf::Vector2f origin = {1, 1}, float Scale = 1, float depth = 1);
        static void Draw(const GameObjectBase::DrawProperties& DrawProperties);

        static void AddDebugCircle(const sf::Vector2f& pos, float radius = 10.f, const sf::Color& color = sf::Color::Red, float thickness = 1.0f);

    private:
        struct SpriteQuad
        {
            sf::Vertex v0;
            sf::Vertex v1;
            sf::Vertex v2;
            sf::Vertex v3;
            const sf::Texture* texture;
            float depth;
            int drawOrder;

            SpriteQuad(const sf::Vertex& v0, const sf::Vertex& v1, const sf::Vertex& v2, const sf::Vertex& v3, const sf::Texture* texture, const float depth, const int drawOrder)
                : v0(v0), v1(v1), v2(v2), v3(v3), texture(texture), depth(depth), drawOrder(drawOrder)
            {
            }
        };

        std::vector<SpriteQuad> sprites;
        int drawCounter = 0;
    };


    template <typename T>
    void SpriteBatch::Draw(const T& drawType, float depth)
    {
        const sf::Texture* texture = drawType.getTexture();
        if (!texture) return;

        const sf::Transform& transform = drawType.getTransform();
        const sf::IntRect& texRect = drawType.getTextureRect();

        sf::Color color;
        if constexpr (std::is_same_v<T, sf::Sprite>) color = drawType.getColor();
        else if constexpr (std::is_same_v<T, sf::RectangleShape>) color = drawType.getOutlineColor();

        sf::Vertex vertices[4];

        // Positions
        vertices[0].position = transform.transformPoint(0.f, 0.f);
        vertices[1].position = transform.transformPoint(static_cast<float>(texRect.width), 0.f);
        vertices[2].position = transform.transformPoint(static_cast<float>(texRect.width), static_cast<float>(texRect.height));
        vertices[3].position = transform.transformPoint(0.f, static_cast<float>(texRect.height));

        // Texture coordinates
        float left = static_cast<float>(texRect.left);
        float right = left + static_cast<float>(texRect.width);
        float top = static_cast<float>(texRect.top);
        float bottom = top + static_cast<float>(texRect.height);

        vertices[0].texCoords = sf::Vector2f(left, top);
        vertices[1].texCoords = sf::Vector2f(right, top);
        vertices[2].texCoords = sf::Vector2f(right, bottom);
        vertices[3].texCoords = sf::Vector2f(left, bottom);

        // Color
        for (auto& vertice : vertices)
            vertice.color = color;

        // Add to the list
        sprites.emplace_back(vertices[0], vertices[1], vertices[2], vertices[3], texture, depth, drawCounter++);
    }


    extern SpriteBatch GlobSpriteBatch;
}
