#include "SpriteBatch.h"

//A forward declaration I've never seen before
namespace ETG
{
    SpriteBatch GlobSpriteBatch;
}


void ETG::SpriteBatch::begin()
{
    sprites.clear();
}

void ETG::SpriteBatch::draw(const sf::Sprite& sprite, float depth)
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
    for (auto& vertice : vertices)
        vertice.color = color;

    // Add to the list
    sprites.emplace_back(vertices[0], vertices[1], vertices[2], vertices[3], texture, depth, drawCounter++);
}

void ETG::SpriteBatch::end(sf::RenderWindow& window)
{
    if (sprites.empty()) return;

    // Sort sprites by draw order. If draw order same, draw the one has higher order. 
    std::sort(sprites.begin(), sprites.end(),
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
    // sprites.clear();  // Remove this line (already cleared in begin())
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

    ETG::GlobSpriteBatch.draw(frame, depth);
}
