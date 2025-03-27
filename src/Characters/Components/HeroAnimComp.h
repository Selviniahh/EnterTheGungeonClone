#pragma once
#include <SFML/Graphics.hpp>
#include "../../Core/Components/BaseAnimComp.h"
#include "../../Characters/Hero.h"

namespace ETG
{
    class HeroAnimComp : public BaseAnimComp<HeroStateEnum>
    {
    public:
        HeroAnimComp();

        //Override
        void Update() override;
        void SetAnimations() override;

        void StartDash(HeroDashEnum direction);
        void EndDash();
        bool IsDashAnimFinished() const;

        //Dash state
        bool IsDashing = false;
        float DashAnimFrameInterval = 0.10;
        float IdleAnimFrameInterval = 0.15;
        float RunAnimFrameInterval = 0.15;
        HeroDashEnum CurrentDashDirection = HeroDashEnum::Unknown;
    
    private:
        Hero* HeroPtr = nullptr;

        BOOST_DESCRIBE_CLASS(HeroAnimComp, (BaseAnimComp), (HeroPtr,DashAnimFrameInterval,IdleAnimFrameInterval, RunAnimFrameInterval, CurrentDashDirection), (), ())
    };
}
