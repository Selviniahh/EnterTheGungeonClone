#pragma once
#include "../Base/GunBase.h"

namespace ETG
{
    class CollisionComponent;
    class AK47 : public GunBase
    {
    public:
        explicit AK47(const sf::Vector2f& pos);
        ~AK47() override = default;

        void Initialize() override;
        void Update() override;

        std::unique_ptr<CollisionComponent> CollisionComp;

        BOOST_DESCRIBE_CLASS(AK47, (GunBase), (), (),());
    };

    class AK47AnimComp : public BaseAnimComp<GunStateEnum>
    {
    public:
        AK47AnimComp();
        void SetAnimations() override;
        BOOST_DESCRIBE_CLASS(AK47AnimComp, (BaseAnimComp), (),(),());
    };
}
