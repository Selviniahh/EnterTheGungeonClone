#pragma once
#include <string>

namespace ETG
{
    class IGunModifier
    {
    public:
        virtual ~IGunModifier() = default;
        [[nodiscard]] virtual std::string GetModifierName() const = 0;
    };
}
