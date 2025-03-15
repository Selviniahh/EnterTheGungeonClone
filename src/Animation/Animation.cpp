#include "Animation.h"
#include "../Managers/Globals.h"
#include "../Managers/SpriteBatch.h"
#include <memory>

Animation::Animation(const std::shared_ptr<sf::Texture>& texture, const float eachFrameSpeed, const int frameX, const int frameY, const int row)
    : EachFrameSpeed(eachFrameSpeed), AnimTimeLeft(EachFrameSpeed), FrameX(frameX), FrameY(frameY), Texture(texture)
{
    const int frameXSize = Texture->getSize().x / frameX;
    const int frameYSize = Texture->getSize().y / frameY;

    for (int i = 0; i < frameX; ++i)
    {
        int frameWidth = i * frameXSize;
        int frameHeight = (row - 1) * frameYSize;

        FrameRects.emplace_back(frameWidth, frameHeight, frameXSize, frameYSize);
    }
}

void Animation::Update()
{
    if (!Active) return;
    AnimTimeLeft -= ETG::Globals::FrameTick;
    if (AnimTimeLeft <= 0)
    {
        CurrentFrame++;
        CurrentFrame = CurrentFrame >= FrameX ? 0 : CurrentFrame;
        AnimTimeLeft = EachFrameSpeed;
    }

    CurrRect = FrameRects[CurrentFrame];
}

void Animation::Draw(const sf::Vector2f position, const float layerDepth, const float rotation) const
{
    if (!Active || !Texture) return;
    
    sf::Sprite frame;
    frame.setTexture(*Texture);
    frame.setTextureRect(FrameRects[CurrentFrame]);
    frame.setPosition(position);
    frame.setColor(sf::Color::White);
    frame.setRotation(rotation);
    frame.setOrigin(Origin);
    frame.setScale(ETG::Globals::DefaultScale * flipX, ETG::Globals::DefaultScale);

    ETG::GlobSpriteBatch.draw(frame, layerDepth);
}

void Animation::Draw(const std::shared_ptr<sf::Texture>& texture, const sf::Vector2f position, const sf::Color color, const float rotation, const sf::Vector2f origin, const sf::Vector2f scale, const float depth) const
{
    if (!Active || !Texture) return;

    sf::Sprite frame;
    frame.setTexture(*Texture);
    frame.setTextureRect(FrameRects[CurrentFrame]);
    frame.setPosition(position);
    frame.setColor(color);
    frame.rotate(rotation);
    frame.setOrigin(origin);
    frame.setScale(scale);

    ETG::GlobSpriteBatch.draw(frame, depth);
}

void Animation::Restart()
{
    CurrentFrame = 0;
    AnimTimeLeft = EachFrameSpeed;
}

std::shared_ptr<sf::Texture> Animation::GetCurrentFrameAsTexture() const
{
    if (!Texture) return nullptr;
    
    // Ensure the cache is large enough
    if (textureCache.size() <= CurrentFrame) textureCache.resize(CurrentFrame + 1);

    // Create texture if it doesn't exist yet
    if (!textureCache[CurrentFrame] || textureCache[CurrentFrame]->getSize().x == 0)
    {
        const sf::IntRect sourceRectangle = FrameRects[CurrentFrame];
        sf::Image frameImage;
        frameImage.create(sourceRectangle.width, sourceRectangle.height);
        frameImage.copy(Texture->copyToImage(), 0, 0, sourceRectangle);
        
        textureCache[CurrentFrame] = std::make_shared<sf::Texture>();
        textureCache[CurrentFrame]->loadFromImage(frameImage);
    }
    return textureCache[CurrentFrame];
}

bool Animation::IsAnimationFinished() const
{
    return CurrentFrame == FrameX - 1;
}

Animation Animation::CreateSpriteSheet(const std::string& RelativePath, const std::string& FileName, const std::string& Extension, const float eachFrameSpeed, bool IsSingleSprite)
{
    // Initial setup
    std::vector<sf::Image> imageArr;
    int counter = 0;
    int totalWidth = 0, maxHeight = 0;
    std::string basePath = (std::filesystem::path(RESOURCE_PATH) / RelativePath / FileName).string();
    char LastChar = basePath[basePath.length() - 1];
    std::string filePath;

    // If lastChar is number, it means it will be a spritesheet. If not the spritesheet will be single.
    if (static_cast<int>(LastChar) >= 48 && static_cast<int>(LastChar) <= 57)
    {
        counter = LastChar - '0';
        basePath[basePath.length() - 1] = counter + '0';
        basePath.erase(basePath.length() - 1, basePath.length() - 2);
        filePath = basePath + std::to_string(counter) + "." + Extension;
    }
    else
    {
        filePath = basePath + "." + Extension;
    }

    // Check firstly if filepath is valid
    if (!std::filesystem::exists(filePath)) throw std::runtime_error("File not found at: " + filePath);

    // Load all the given textures 
    while (true)
    {
        sf::Image singleImage;

        // If the sprite is single, just load and exit the loop. If it's multiple sprites, load all 
        IsSingleSprite ? filePath = basePath + "." + Extension : filePath = basePath + std::to_string(counter) + "." + Extension; 
        
        if (!std::filesystem::exists(filePath)) break;

        if (!singleImage.loadFromFile(filePath))
            throw std::runtime_error("Failed to load image: " + filePath);

        imageArr.push_back(singleImage);
        counter++;

        totalWidth += int(singleImage.getSize().x);
        maxHeight = std::max(maxHeight, int(singleImage.getSize().y));

        if (IsSingleSprite) break;
    }

    // Create the spritesheet as image
    sf::Image spriteImage;
    spriteImage.create(totalWidth, maxHeight);

    // Copy images into the image
    unsigned int xOffset = 0;
    for (auto& image : imageArr)
    {
        spriteImage.copy(image, xOffset, 0);
        xOffset += image.getSize().x;
    }

    // Convert image into texture and draw it
    auto spriteTex = std::make_shared<sf::Texture>();
    spriteTex->loadFromImage(spriteImage);

    auto anim = Animation{spriteTex, eachFrameSpeed, int(imageArr.size()), 1};
    anim.AnimPathName = RelativePath + FileName;
    return anim;
}