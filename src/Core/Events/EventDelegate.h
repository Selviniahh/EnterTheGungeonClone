#pragma once
#include <functional>
#include <vector>
#include <map> // Or unordered_map
#include <atomic> // For thread-safe handle generation if needed, simpler counter otherwise

namespace ETG
{
    template <typename... Args>
    class EventDelegate
    {
    public:
        using Callback = std::function<void(Args...)>;
        // Define a type for the listener handle (can be simple int/size_t)
        using ListenerHandle = size_t;
        constexpr static ListenerHandle InvalidHandle = -1; // Or similar invalid value

        // Add a listener and return a handle
        ListenerHandle AddListener(Callback callBack)
        {
            ListenerHandle handle = nextHandle++;
            // Store handle along with the callback
            Listeners.emplace(handle, std::move(callBack));
            return handle;
        }

        // Remove a specific listener using its handle
        void RemoveListener(ListenerHandle handle)
        {
            Listeners.erase(handle);
        }

        // Broadcast the event to all listeners
        void Broadcast(Args... args)
        {
            // Iterate over map values (callbacks)
            for (const auto& pair : Listeners)
            {
                pair.second(args...); // Call the callback
            }
        }

        // Clear all listeners (keep this if needed, but use RemoveListener for specific cases)
        void Clear()
        {
            Listeners.clear();
        }

    private:
        // Use a map to associate handles with callbacks
        std::map<ListenerHandle, Callback> Listeners;
        // Simple counter for generating unique handles
        ListenerHandle nextHandle = 0;
    };
} // namespace ETG