// Managers/SpriteBatch.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class SpriteBatch
{
public:
    SpriteBatch() = default;

    void begin()
    {
        sprites.clear();
    }

    //NOTE: Sprites added to a batch are transformed based on the active view 
    void draw(const sf::Sprite& sprite)
    {
        const sf::Texture* texture = sprite.getTexture();
        if (!texture) return;

        const sf::Transform& transform = sprite.getTransform();
        const sf::IntRect& texRect = sprite.getTextureRect();
        const sf::Color color = sprite.getColor();

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
        for (auto & vertice : vertices)
            vertice.color = color;

        // Add to the list
        sprites.emplace_back(vertices[0], vertices[1], vertices[2], vertices[3], texture);
    }

    void end(sf::RenderWindow& window) {
        if (sprites.empty()) return;

        // Sort sprites by texture to minimize switches. NOTE THAT it's sorting based on given memory addresses
        std::sort(sprites.begin(), sprites.end(), 
            [](const SpriteQuad& a, const SpriteQuad& b) { return a.texture < b.texture; });

        const sf::Texture* currentTexture = sprites[0].texture;
        sf::VertexArray va(sf::Quads);
        va.resize(sprites.size() * 4); // Reserve if possible

        for (const auto& quad : sprites) {
            if (quad.texture != currentTexture) {
                window.draw(va, currentTexture);
                va.clear();
                currentTexture = quad.texture;
            }

            va.append(quad.v0);
            va.append(quad.v1);
            va.append(quad.v2);
            va.append(quad.v3);
        }

        window.draw(va, currentTexture);
        // sprites.clear();  // Remove this line (already cleared in begin())
    }

private:
    struct SpriteQuad
    {
        sf::Vertex v0;
        sf::Vertex v1;
        sf::Vertex v2;
        sf::Vertex v3;
        const sf::Texture* texture;

        SpriteQuad(const sf::Vertex& v0, const sf::Vertex& v1, const sf::Vertex& v2, const sf::Vertex& v3, const sf::Texture* texture)
            : v0(v0), v1(v1), v2(v2), v3(v3), texture(texture)
        {
        }
    };

    std::vector<SpriteQuad> sprites;
};