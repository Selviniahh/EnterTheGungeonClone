#include "Misc.h"
#include <numbers>

float ETG::Misc::RadiansToDegrees(const float radians)
{
    return radians * (180.0f / std::numbers::pi); 
}
