#pragma once
#include <functional>
#include <vector>

namespace ETG
{
    //A simple, lightweight event delegate system
    template <typename... Args>
    class EventDelegate
    {
    public:
        using Callback = std::function<void(Args...)>;

        //Add a listener to the event
        void AddListener(Callback callBack)
        {
            CallBacks.push_back(callBack);
        }

        //Broadcast the event to all listeners
        void Broadcast(Args... args)
        {
            for (const auto& callBack : CallBacks)
            {
                callBack(args...);
            }
        }

        //Clear all listeners
        void Clear()
        {
            CallBacks.clear();
        }

    private:
        std::vector<Callback> CallBacks;
    };
}
