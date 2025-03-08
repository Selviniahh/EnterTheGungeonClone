#pragma once
#include <boost/describe.hpp>

namespace ETG
{
    enum class RunEnum
    {
        Run_Back,
        Run_BackWard,
        Run_Forward,
        Run_Front
    };
    BOOST_DESCRIBE_ENUM(RunEnum, Run_Back, Run_BackWard, Run_Forward, Run_Front)

    enum class IdleEnum
    {
        Idle_Back,
        Idle_BackWard,
        Idle_Front,
        Idle_Right
    };
    BOOST_DESCRIBE_ENUM(IdleEnum,Idle_Back, Idle_BackWard, Idle_Front, Idle_Right)

    enum class DashEnum
    {
        Dash_Back,
        Dash_BackWard,
        Dash_Front,
        Dash_Left,
        Dash_Right,
        Unknown
    };
    BOOST_DESCRIBE_ENUM(DashEnum, Dash_Back, Dash_BackWard, Dash_Front, Dash_Left, Dash_Right, Unknown)

    enum class HeroStateEnum
    {
        Idle,
        Run,
        Dash,
        Die
    };
    BOOST_DESCRIBE_ENUM(HeroStateEnum, Idle, Run, Dash, Die)
    
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

    static std::vector<ETG::RunEnum> RunEnumValues = {
        RunEnum::Run_Back,
        RunEnum::Run_BackWard,
        RunEnum::Run_Forward,
        RunEnum::Run_Front
    };

    static std::vector<ETG::IdleEnum> IdleEnumValues
    {
        IdleEnum::Idle_Back,
        IdleEnum::Idle_BackWard,
        IdleEnum::Idle_Front,
        IdleEnum::Idle_Right,
    };

    static std::vector<ETG::DashEnum> DashEnumValues = {

        DashEnum::Dash_Back,
        DashEnum::Dash_BackWard,
        DashEnum::Dash_Front,
        DashEnum::Dash_Left,
        DashEnum::Dash_Right
    };
}
