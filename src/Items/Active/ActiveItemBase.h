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
    enum class ActiveItemState;

    class ComponentBase;

    class ActiveItemBase : public GameObjectBase
    {
    public:
        explicit ActiveItemBase(const std::string& resourcePath, const std::string& activateResPath, const float& cooldownTime, const float& activeTime);

    public:
        float TotalCooldownTime;
        float TotalConsumeTime;
        float ConsumeTimer; //Will be increased when the item is consuming
        float CoolDownTimer; //Will be increased when the item is in cooldown
        ActiveItemState ActiveItemState{};

        sf::SoundBuffer ActivateSoundBuffer;
        sf::Sound ActivateSound;

        sf::SoundBuffer ReadySoundBuffer;
        sf::Sound ReadySound;

        virtual void RequestUsage();
        void Update() override;

    protected:
        void PlayRandomPickupSound();

        std::string ItemDescription{};

        std::array<sf::SoundBuffer, 2> PickupSoundBuffers;
        std::array<sf::Sound, 2> PickupSounds;

        //Random number generator
        std::mt19937 rng{std::random_device{}()};

        BOOST_DESCRIBE_CLASS(ActiveItemBase, (GameObjectBase), (TotalCooldownTime, TotalConsumeTime, ConsumeTimer, CoolDownTimer, ActiveItemState), (ItemDescription), ())
    };

    enum class ActiveItemState
    {
        Ready, //Item is ready to be consumed
        Consuming, //Item is currently being consumed
        Cooldown //Item is in Cooldown state
    };

    BOOST_DESCRIBE_ENUM(ActiveItemState, Ready, Consuming, Cooldown)
}
