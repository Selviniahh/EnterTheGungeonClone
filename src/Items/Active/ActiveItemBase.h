#pragma once
#include <filesystem>
#include <random>
#include <array>
#include <iostream>
#include "SFML/Audio.hpp"
#include "../../Core/GameObjectBase.h"

//TODO: is active item base class necessary or I should remove it and rename PassiveItemBase to ItemBase and use it as base class for both active and passive classes 
namespace ETG
{
    class ComponentBase;

    class ActiveItemBase : public GameObjectBase
    {
    public:
        explicit ActiveItemBase(const std::string& resourcePath)
        {
            //Load the texture
            Texture = std::make_shared<sf::Texture>();

            if (!Texture->loadFromFile(resourcePath))
                std::cerr << "Failed to load hand texture" << std::endl;

            // Load sound effects
            if (!PickupSoundBuffers[0].loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "Pickup1.wav").generic_string()))
                std::cerr << "Failed to load Pickup1.wav sound" << std::endl;

            if (!PickupSoundBuffers[1].loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "Pickup2.wav").generic_string()))
                std::cerr << "Failed to load Pickup2.wav sound" << std::endl;

            // Connect sounds to their buffers
            Sounds[0].setBuffer(PickupSoundBuffers[0]);
            Sounds[1].setBuffer(PickupSoundBuffers[1]);
        }

    protected:
        void PlayRandomPickupSound()
        {
            std::uniform_int_distribution<int> dist(0, Sounds.size() - 1);
            const int soundIndex = dist(rng);
            Sounds[soundIndex].play();
            IsVisible = false;
        }

        std::string ItemDescription{};

        std::array<sf::SoundBuffer, 2> PickupSoundBuffers;
        std::array<sf::Sound, 2> Sounds;

        //Random number generator
        std::mt19937 rng{std::random_device{}()};

        BOOST_DESCRIBE_CLASS(ActiveItemBase, (GameObjectBase), (), (), ())
    };
}
