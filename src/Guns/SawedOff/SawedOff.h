#pragma once
#include "../Base/GunBase.h"

namespace ETG
{
    class CollisionComponent;
    class SawedOff : public GunBase
    {
    public:
        explicit SawedOff(const sf::Vector2f& pos);
        ~SawedOff() override = default;

        void Initialize() override;
        void Update() override;
        void Draw() override;
        void PrepareShooting() override;

        std::unique_ptr<CollisionComponent> CollisionComp;
        BOOST_DESCRIBE_CLASS(SawedOff, (GunBase), (), (),());
    };

    class SawedOffAnimComp : public BaseAnimComp<GunStateEnum>
    {
    public:
        SawedOffAnimComp();
        void SetAnimations() override;
        BOOST_DESCRIBE_CLASS(SawedOffAnimComp, (BaseAnimComp), (),(),());
    };
}
