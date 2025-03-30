#include <filesystem>
#include "../../Core/Factory.h"
#include "../../Modifiers/Gun/MultiShotModifier.h"
#include "../../Core/Components/CollisionComponent.h"
#include "../../Characters/Hero.h"
#include "SawedOff.h"

ETG::SawedOff::SawedOff(const sf::Vector2f& pos) : GunBase(pos,
1.5f,     // FireRate
100.0f,     // ShotSpeed
200.0f,    // Range (should be infinite but I will just give 2000)
0.0f,      // timerForVelocity
3.0f,      // depth
165,       // MaxAmmo
6,        // MagazineSize
5.0f,      // ReloadTime
5.5f,      // Damage
0.f,      // Force
3.0f)      // Spread (in degrees)
{
    AnimationComp = CreateGameObjectAttached<SawedOffAnimComp>(this);
    SetShootSound((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "AK47Shoot.ogg").generic_string());
    SetReloadSound((std::filesystem::path(RESOURCE_PATH) / "Sounds" / "AK47Reload.ogg").generic_string());
    
    CollisionComp = ETG::CreateGameObjectAttached<CollisionComponent>(this);
    CollisionComp->CollisionRadius = 1.f;
    CollisionComp->SetCollisionEnabled(true);
    CollisionComp->ShowCollisionBounds = true;

    SawedOff::Initialize();
}


void ETG::SawedOff::Initialize()
{
    GunBase::Initialize();
    Timer = FireRate + 1; //Set timer to be greater than fire rate so that we can shoot immediately
    OriginOffset = {1.f, 5.f};
    ArrowComp->arrowOriginOffset = {-6.f, 0.f};
    ArrowComp->arrowOffset = {15.f, -4.f};
    CollisionComp->Initialize();

    const auto projPath = (std::filesystem::path(RESOURCE_PATH) / "Projectiles"  / "bullet_variant_003.png").string();
    if (!ProjTexture->loadFromFile(projPath))
        throw std::runtime_error("Failed to load Projectile_SawedOff.png from path: " + projPath);

    //For now inside the gun if it collides with hero, hero will pick it up. We can do it instead inside hero later 
    CollisionComp->OnCollisionEnter.AddListener([this](const CollisionEventData& eventData)
    {
        if (auto* hero = dynamic_cast<Hero*>(eventData.Other))
        {
            hero->EquipGun(this);
            CollisionComp->SetCollisionEnabled(false); //After equip 
        }
    });

    GunBase::Initialize();
}

void ETG::SawedOff::Update()
{
    MuzzleFlash->Deactivate();
    MuzzleFlash->IsVisible = false;
    CollisionComp->Update();
    ArrowComp->Update();
    GunBase::Update();
}

void ETG::SawedOff::Draw()
{
    GunBase::Draw();
    if (CollisionComp) CollisionComp->Visualize(*GameState::GetInstance().GetRenderWindow());
}

//Because we will do something different, we need to override this function without calling base function
void ETG::SawedOff::PrepareShooting()
{
    if (Timer >= FireRate)
    {
        //Reset firing timer
        Timer = 0;

        //NOTE: DO NOT MISS HERE!!!!!!! Apply modifiers if present. Later on when all of these modifiers will get complex, a new class that will only handle modifiers should be created. 
        int shotCount = 1;
        float EffectiveSpread = Spread; //Because Spread needs to be reverted back, when modifier overs, in place temp local variable required 
        if (const auto& multiMod = modifierManager.GetModifier<MultiShotModifier>())
        {
            shotCount = multiMod->GetShotCount();
            EffectiveSpread = multiMod->GetSpread();
        }

        //Consume ammo only once per shot group regardless of MultiShotModifier
        MagazineAmmo--;

        //Queue any additional bullets with delay
        for (int i = 0; i < shotCount; i++)
        {
            float projectileAngle = Rotation;

            //Queue the bullet
            bulletQueue.push_back({i * MULTI_SHOT_DELAY, projectileAngle});
            bulletQueue.push_back({i * MULTI_SHOT_DELAY, projectileAngle - 15});
            bulletQueue.push_back({i * MULTI_SHOT_DELAY, projectileAngle + 15});
        }
    }

    //Handle ammo depletion
    if (MagazineAmmo == 0)
    {
        OnAmmoRunOut.Broadcast(true);
    }
}

ETG::SawedOffAnimComp::SawedOffAnimComp()
{
    IsGameObjectUISpecified = true;
    SawedOffAnimComp::SetAnimations();
}

void ETG::SawedOffAnimComp::SetAnimations()
{
    BaseAnimComp<GunStateEnum>::SetAnimations();
    // Idle Animation
    const Animation IdleAnim = {Animation::CreateSpriteSheet("Guns/SawedOff", "sawed_off_shotgun_idle_001", "png", 0.15f, false)};
    IdleAnim.Origin = {45.f, 5.f};
    AddGunAnimationForState(GunStateEnum::Idle, IdleAnim, {45.f, 5.f});

    // Shoot Animations
    const Animation ShootAnim = {Animation::CreateSpriteSheet("Guns/SawedOff", "sawed_off_shotgun_fire_001", "png", 0.15f)};
    ShootAnim.Origin = {1.f, 5.f};
    AddGunAnimationForState(GunStateEnum::Shoot, ShootAnim, {1.f, 5.f});

    // Reload Animation
    const Animation ReloadAnim = {Animation::CreateSpriteSheet("Guns/SawedOff", "sawed_off_shotgun_reload_001", "png", 0.25f, false)};
    ReloadAnim.Origin = {45.f, 5.f};
    AddGunAnimationForState(GunStateEnum::Reload, ReloadAnim, {50.f, 5.f}); //IDK why origin not changing. 
    
}
