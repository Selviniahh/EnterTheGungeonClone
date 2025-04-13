#pragma once
#include "../Base/GunBase.h"

namespace ETG
{
    class CollisionComponent;
    class Magnum : public GunBase
    {
    public:
        explicit Magnum(const sf::Vector2f& pos);
        ~Magnum() override = default;
        
        void Initialize() override;
        void Update() override;
        void Draw() override;
        void EnqueueProjectiles(int shotCount, float EffectiveSpread) override;
        
        std::unique_ptr<CollisionComponent> CollisionComp;
        BOOST_DESCRIBE_CLASS(Magnum, (GunBase), (), (), ());
    };

    class MagnumAnimComp : public BaseAnimComp<GunStateEnum>
    {
    public:
        MagnumAnimComp();
        void SetAnimations() override;
        float ShootAnimInterval = 0.1f;
        float ReloadAnimInterval = 3.f / 2.f; //Frame Count / Reload Time = Reload Time; (but of course we need to divide bigger value first as we can't have negative anim interval)  
        
        BOOST_DESCRIBE_CLASS(MagnumAnimComp, (BaseAnimComp), (), (), ())
    };
}