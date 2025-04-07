#pragma once

namespace ETG
{
    enum HeroStateFlags
    {
        StateNone = 0,
        StateIdle = 1 << 0, //1
        StateRun = 1 << 1, //2
        StateDash = 1 << 2, //4
        StateDie = 1 << 3, //8
        StateHit = 1 << 4, //16

        PreventGunSwitching = StateDash | StateDie | StateHit,
        PreventMovement = StateDash | StateDie | StateHit,
        PreventShooting = StateDash | StateDie | StateHit,
        PreventAnimFlip = StateDie | StateHit,
        PreventActiveItemUsage = StateDie | StateHit | StateDash,
        PreventDamage = StateDie | StateHit | StateDash
    };

    //Helper functions for flag manipulation
    inline HeroStateFlags operator|(const HeroStateFlags a, const HeroStateFlags b)
    {
        return static_cast<HeroStateFlags>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline HeroStateFlags operator&(const HeroStateFlags a, const HeroStateFlags b)
    {
        return static_cast<HeroStateFlags>(static_cast<int>(a) & static_cast<int>(b));
    }

    inline bool HasAnyFlag(const HeroStateFlags flags, const HeroStateFlags check)
    {
        return (flags & check) != StateNone;
    }

    inline bool HasAllFlags(const HeroStateFlags flags, const HeroStateFlags check)
    {
        return (flags & check) == check;
    }
}
