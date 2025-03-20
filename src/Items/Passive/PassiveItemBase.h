#pragma once
#include <filesystem>
#include <random>
#include <array>
#include <iostream>
#include "SFML/Audio.hpp"
#include "../../Core/GameObjectBase.h"

namespace ETG
{
    class ComponentBase;

    class PassiveItemBase : public GameObjectBase
    {
    public:
        PassiveItemBase()
        {
            //Load the texture
            Texture = std::make_shared<sf::Texture>();

            if (!Texture->loadFromFile((std::filesystem::path(RESOURCE_PATH) / "Items" / "Passive" / "platinum_bullets_001.png").generic_string()))
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

        std::string ItemDescription{};
        
    protected:
        
        //Why did I choose array over vector? std::aray is fixed-size with zero overhead. There's no dynamic memory allocation.
        //It's stack allocated. So use std::array if the collection size is known at compile time
        std::array<sf::SoundBuffer, 2> PickupSoundBuffers;
        std::array<sf::Sound, 2> Sounds;

        //Random number generator
        std::mt19937 rng{std::random_device{}()};

        BOOST_DESCRIBE_CLASS(PassiveItemBase, (GameObjectBase), (), (), ())
    };
}
