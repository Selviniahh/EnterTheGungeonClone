#include "SpriteBatch.h"

#include "Globals.h"
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

    GlobSpriteBatch.Draw(frame, depth);
}

void ETG::SpriteBatch::Draw(const GameObjectBase::DrawProperties& DrawProperties)
{
    if (!DrawProperties.Texture) return;
    
    sf::Sprite frame;
    frame.setTexture(*DrawProperties.Texture);
    frame.setScale(DrawProperties.Scale);
    frame.setPosition(DrawProperties.Position); // Position it at the specified location
    frame.setRotation(DrawProperties.Rotation);
    frame.setOrigin(DrawProperties.Origin);
    frame.setColor(DrawProperties.Color);
    GlobSpriteBatch.Draw(frame, DrawProperties.Depth);

}

void ETG::SpriteBatch::AddDebugCircle(const sf::Vector2f& pos, const float radius, const sf::Color& color, const float thickness)
{
    sf::CircleShape circle{radius};
    circle.setPosition(pos.x - radius, pos.y - radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(color);
    circle.setOutlineThickness(thickness);
    Globals::Window->draw(circle);
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
    Draw(topEdge, depth);
    
    // Bottom edge
    sf::Sprite bottomEdge;
    bottomEdge.setTexture(*pixelTex);
    bottomEdge.setPosition(rect.left, rect.top + rect.height - thickness);
    bottomEdge.setScale(rect.width, thickness);
    bottomEdge.setColor(color);
    Draw(bottomEdge, depth);
    
    // Left edge
    sf::Sprite leftEdge;
    leftEdge.setTexture(*pixelTex);
    leftEdge.setPosition(rect.left, rect.top);
    leftEdge.setScale(thickness, rect.height);
    leftEdge.setColor(color);
    Draw(leftEdge, depth);
    
    // Right edge
    sf::Sprite rightEdge;
    rightEdge.setTexture(*pixelTex);
    rightEdge.setPosition(rect.left + rect.width - thickness, rect.top);
    rightEdge.setScale(thickness, rect.height);
    rightEdge.setColor(color);
    Draw(rightEdge, depth);
}
