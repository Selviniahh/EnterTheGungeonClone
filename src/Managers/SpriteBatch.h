// Managers/SpriteBatch.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Globals.h"

namespace ETG
{
    class SpriteBatch
    {
    public:
        SpriteBatch() = default;

        void begin();

        //NOTE: Sprites added to a batch are transformed based on the active view 
        void draw(const sf::Sprite& sprite, float depth);

        void end(sf::RenderWindow& window);

        static void SimpleDraw(const sf::Texture& tex, const sf::Vector2f& pos, float Rotation = 0, sf::Vector2f origin = {1, 1}, float Scale = 1, float depth = 1);

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

    extern SpriteBatch GlobSpriteBatch;
}
