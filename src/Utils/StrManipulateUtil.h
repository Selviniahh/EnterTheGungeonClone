#pragma once
#include <string>

namespace ETG
{
    // //Remove everything up to and including the last ::
    static void RemoveNamespaceFromName(std::string& name)
    {
        const size_t LastColon = name.find_last_of("::");
        if (LastColon != std::string::npos)
        {
            name = name.substr(LastColon + 1);
        }
    }
}
