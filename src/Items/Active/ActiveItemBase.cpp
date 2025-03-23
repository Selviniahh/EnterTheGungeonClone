#include "ActiveItemBase.h"

#include "../../Managers/Globals.h"

ETG::ActiveItemBase::ActiveItemBase(const std::string& resourcePath, const float& cooldownTime, const float& activeTime) : TotalCooldownTime(cooldownTime), TotalConsumeTime(activeTime)
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

void ETG::ActiveItemBase::RequestUsage()
{
}

void ETG::ActiveItemBase::Update()
{
    GameObjectBase::Update();

    //Just increment the corresponding timer normally first
    if (ActiveItemState == ActiveItemState::Consuming) ConsumeTimer += Globals::FrameTick;
    if (ActiveItemState == ActiveItemState::Cooldown) CoolDownTimer += Globals::FrameTick;

    //If current state is consuming and consume time is up, set ActiveItemState to cooldown state 
    if (ActiveItemState == ActiveItemState::Consuming && ConsumeTimer >= TotalConsumeTime)
    {
        ActiveItemState = ActiveItemState::Cooldown;
        ConsumeTimer = 0;
        CoolDownTimer = 0;
    }

    //If current state is cooldown and cooldown timer is up, set current state to be ready
    else if (ActiveItemState == ActiveItemState::Cooldown && CoolDownTimer >= TotalCooldownTime)
    {
        ActiveItemState = ActiveItemState::Ready;
        ConsumeTimer = 0;
        CoolDownTimer = 0;
    }
}

void ETG::ActiveItemBase::PlayRandomPickupSound()
{
    std::uniform_int_distribution<int> dist(0, Sounds.size() - 1);
    const int soundIndex = dist(rng);
    Sounds[soundIndex].play();
    IsVisible = false;
}
