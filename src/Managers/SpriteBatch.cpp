#include "SpriteBatch.h"
#include "../Utils/TextureUtils.h"

//A forward declaration I've never seen before
namespace ETG
{
    SpriteBatch GlobSpriteBatch;
}


void ETG::SpriteBatch::begin()
{
    sprites.clear();
}

void ETG::SpriteBatch::end(sf::RenderWindow& window)
{
    if (sprites.empty()) return;

    // Sort sprites by draw order. If draw order same, draw the one has higher order. 
    std::ranges::sort(sprites,
                      [](const SpriteQuad& a, const SpriteQuad& b)
                      {
                          if (a.depth == b.depth)
                              return a.drawOrder < b.drawOrder;
                          return a.depth > b.depth;
                      });

    const sf::Texture* currentTexture = sprites[0].texture;
    sf::VertexArray va(sf::Quads);
    va.resize(sprites.size() * 4); // Reserve if possible

    for (const auto& quad : sprites)
    {
        if (quad.texture != currentTexture)
        {
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
}

void ETG::SpriteBatch::SimpleDraw(const std::shared_ptr<sf::Texture>& tex, const sf::Vector2f& pos, float Rotation, sf::Vector2f origin, float Scale, float depth)
{
    sf::Sprite frame;
    frame.setTexture(*tex);
    frame.setScale(Scale, Scale);
    frame.setPosition(pos); // Position it at the specified location
    frame.setRotation(Rotation);
    frame.setOrigin(origin);
    frame.setColor(sf::Color::White);

    GlobSpriteBatch.draw(frame, depth);
}

void ETG::SpriteBatch::drawRectOutline(const sf::FloatRect& rect, const sf::Color& color, float thickness, float depth)
{
    // Get a shared pixel texture
    static std::shared_ptr<sf::Texture> pixelTex = GetPixelTexture();
    
    // Top edge
    sf::Sprite topEdge;
    topEdge.setTexture(*pixelTex);
    topEdge.setPosition(rect.left, rect.top);
    topEdge.setScale(rect.width, thickness);
    topEdge.setColor(color);
    draw(topEdge, depth);
    
    // Bottom edge
    sf::Sprite bottomEdge;
    bottomEdge.setTexture(*pixelTex);
    bottomEdge.setPosition(rect.left, rect.top + rect.height - thickness);
    bottomEdge.setScale(rect.width, thickness);
    bottomEdge.setColor(color);
    draw(bottomEdge, depth);
    
    // Left edge
    sf::Sprite leftEdge;
    leftEdge.setTexture(*pixelTex);
    leftEdge.setPosition(rect.left, rect.top);
    leftEdge.setScale(thickness, rect.height);
    leftEdge.setColor(color);
    draw(leftEdge, depth);
    
    // Right edge
    sf::Sprite rightEdge;
    rightEdge.setTexture(*pixelTex);
    rightEdge.setPosition(rect.left + rect.width - thickness, rect.top);
    rightEdge.setScale(thickness, rect.height);
    rightEdge.setColor(color);
    draw(rightEdge, depth);
}
