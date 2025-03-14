#pragma once
#include <boost/describe.hpp>

namespace ETG
{
    enum class HeroRunEnum
    {
        Run_Back,
        Run_BackWard,
        Run_Forward,
        Run_Front
    };
    BOOST_DESCRIBE_ENUM(HeroRunEnum, Run_Back, Run_BackWard, Run_Forward, Run_Front)

    enum class HeroIdleEnum
    {
        Idle_Back,
        Idle_BackWard,
        Idle_Front,
        Idle_Right
    };
    BOOST_DESCRIBE_ENUM(HeroIdleEnum,Idle_Back, Idle_BackWard, Idle_Front, Idle_Right)

    enum class HeroDashEnum
    {
        Dash_Back,
        Dash_BackWard,
        Dash_Front,
        Dash_Left,
        Dash_Right,
        Unknown
    };
    BOOST_DESCRIBE_ENUM(HeroDashEnum, Dash_Back, Dash_BackWard, Dash_Front, Dash_Left, Dash_Right, Unknown)

    enum class HeroStateEnum
    {
        Idle,
        Run,
        Dash,
        Die
    };
    BOOST_DESCRIBE_ENUM(HeroStateEnum, Idle, Run, Dash, Die)

    enum class EnemyStateEnum
    {
        Idle,
        Run,
        Dash,
        Die
    };
    BOOST_DESCRIBE_ENUM(EnemyStateEnum, Idle, Run, Dash, Die)

    enum class BulletManRunEnum
    {
        Run_Left,
        Run_Left_Back,
        Run_Right,
        Run_Right_Back
    };
    BOOST_DESCRIBE_ENUM(BulletManRunEnum, Run_Left, Run_Left_Back, Run_Right, Run_Right_Back)

    enum class BulletManIdleEnum
    {
        Idle_Back,
        Idle_Right,
        Idle_Left
    };
    BOOST_DESCRIBE_ENUM(BulletManIdleEnum, Idle_Back, Idle_Left, Idle_Right)
    
    enum class GunStateEnum
    {
        Idle,
        Shoot,
        Reload
    };
    BOOST_DESCRIBE_ENUM(GunStateEnum, Idle, Shoot, Reload)

    enum class Direction
    {
        Right,
        FrontHandRight,
        FrontHandLeft,
        Left,
        BackDiagonalLeft,
        BackHandLeft,
        BackHandRight,
        BackDiagonalRight
    };
    BOOST_DESCRIBE_ENUM(Direction, Right, FrontHandRight, FrontHandLeft, Left, BackDiagonalLeft, BackHandLeft, BackHandRight, BackDiagonalRight)

    static std::vector<ETG::HeroRunEnum> HeroRunEnumValues = {
        HeroRunEnum::Run_Back,
        HeroRunEnum::Run_BackWard,
        HeroRunEnum::Run_Forward,
        HeroRunEnum::Run_Front
    };

    static std::vector<ETG::HeroIdleEnum> HeroIdleEnumValues
    {
        HeroIdleEnum::Idle_Back,
        HeroIdleEnum::Idle_BackWard,
        HeroIdleEnum::Idle_Front,
        HeroIdleEnum::Idle_Right,
    };

    static std::vector<ETG::HeroDashEnum> HeroDashEnumValues = {

        HeroDashEnum::Dash_Back,
        HeroDashEnum::Dash_BackWard,
        HeroDashEnum::Dash_Front,
        HeroDashEnum::Dash_Left,
        HeroDashEnum::Dash_Right
    };

    static std::vector<ETG::BulletManRunEnum> BulletManRunEnumValues = {
        BulletManRunEnum::Run_Left,
        BulletManRunEnum::Run_Left_Back,
        BulletManRunEnum::Run_Right,
        BulletManRunEnum::Run_Right_Back
    };

    static std::vector<ETG::BulletManIdleEnum> BulletManIdleEnumValues
    {
        BulletManIdleEnum::Idle_Back,
        BulletManIdleEnum::Idle_Right,
        BulletManIdleEnum::Idle_Left
    };

}
