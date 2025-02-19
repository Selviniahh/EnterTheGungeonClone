#include "DirectionUtils.h"
#include <stdexcept>
#include "../Managers/StateEnums.h"

void ETG::DirectionUtils::PopulateDirectionRanges(DirectionMap mapToFill)
{
    mapToFill[{0, 55}] = Direction::Right;
    mapToFill[{55, 75}] = Direction::FrontHandRight;
    mapToFill[{75, 115}] = Direction::FrontHandLeft;
    mapToFill[{115, 190}] = Direction::Left;
    mapToFill[{190, 245}] = Direction::BackDiagonalLeft;
    mapToFill[{245, 265}] = Direction::BackHandLeft;
    mapToFill[{265, 290}] = Direction::BackHandRight;
    mapToFill[{290, 310}] = Direction::BackDiagonalRight;
    mapToFill[{310, 360}] = Direction::Right;
}

ETG::Direction ETG::DirectionUtils::GetDirectionFromAngle(const std::unordered_map<std::pair<int, int>, Direction, PairHash>& DirectionMap, const float angle)
{
    for (auto& entry : DirectionMap)
    {
        //Check if angle within any defined range
        if (angle >= entry.first.first && angle <= entry.first.second)
        {
            return entry.second;
        }
    }
    throw std::out_of_range("Mouse angle is out of defined ranges. Angle is: " + std::to_string(angle));
}

ETG::IdleEnum ETG::DirectionUtils::GetIdleDirectionEnum(Direction currDir)
{
    if (currDir == Direction::BackHandRight || currDir == Direction::BackHandLeft) return IdleEnum::Idle_Back;
    if (currDir == Direction::BackDiagonalRight || currDir == Direction::BackDiagonalLeft) return IdleEnum::Idle_BackWard;
    if (currDir == Direction::Right || currDir == Direction::Left) return IdleEnum::Idle_Right;
    if (currDir == Direction::FrontHandRight || currDir == Direction::FrontHandLeft) return IdleEnum::Idle_Front;
    return IdleEnum::Idle_Back; // Default case
}

ETG::RunEnum ETG::DirectionUtils::GetRunEnum(Direction currDir)
{
    if (currDir == Direction::BackHandRight || currDir == Direction::BackHandLeft) return RunEnum::Run_Back;
    if (currDir == Direction::BackDiagonalRight || currDir == Direction::BackDiagonalLeft) return RunEnum::Run_BackWard;
    if (currDir == Direction::Right || currDir == Direction::Left) return RunEnum::Run_Forward;
    if (currDir == Direction::FrontHandRight || currDir == Direction::FrontHandLeft) return RunEnum::Run_Front;
    return RunEnum::Run_Forward; // Default case
}

std::string ETG::DirectionUtils::StringifyDirection(Direction dir)
{
    switch (dir)
    {
    case Direction::Right: return "Right";
    case Direction::FrontHandRight: return "FrontHandRight";
    case Direction::FrontHandLeft: return "FrontHandLeft";
    case Direction::Left: return "Left";
    case Direction::BackDiagonalLeft: return "BackDiagonalLeft";
    case Direction::BackHandLeft: return "BackHandLeft";
    case Direction::BackHandRight: return "BackHandRight";
    case Direction::BackDiagonalRight: return "BackDiagonalRight";
    }
    // If there’s no match, return something
    return "Unknown Direction";
}
