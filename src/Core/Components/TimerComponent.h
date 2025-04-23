#pragma once
#include "../GameClass.h"
#include "../Events/EventDelegate.h"
#include "../../Managers/Globals.h"
#include "../ComponentBase.h"

namespace ETG
{
    enum class TimeUnit
    {
        Nanosecond,
        Microsecond,
        Millisecond,
        Second,
        Minute,
        Hour,
        Day,
        Week,
        Year,
        Month
    };
    BOOST_DESCRIBE_ENUM(TimeUnit,Nanosecond, Microsecond, Millisecond, Second, Minute, Hour, Day, Week, Year, Month)
    
    class TimerComponent : public ComponentBase 
    {
    public:
        // Constructor that takes total time in seconds
        explicit TimerComponent(float totalTimeSeconds, TimeUnit unit = TimeUnit::Second);
        ~TimerComponent() override;

        // Update function to be called every frame
        void Update() override;

        // Start/Stop/Reset controls
        void Start();
        void Stop();
        void Reset();
        void Restart(); // Combines Reset() and Start()

        [[nodiscard]] bool IsFinished() const;
        [[nodiscard]] bool IsRunning() const;

        // Get remaining and elapsed time
        [[nodiscard]] float GetRemainingTime(TimeUnit unit = TimeUnit::Second) const;
        [[nodiscard]] float GetElapsedTime(TimeUnit unit = TimeUnit::Second) const;

        // Get percentage complete (0.0 to 1.0)
        [[nodiscard]] float GetProgress() const;

        void SetDuration(float newDuration, TimeUnit unit = TimeUnit::Second);

        // Delegate that broadcasts when timer completes
        EventDelegate<> OnTimerFinished;
        EventDelegate<> OnTimerStarted;


    private:
        // Convert between time units
        [[nodiscard]] static float ConvertTimeUnit(float time, TimeUnit fromUnit, TimeUnit toUnit);
        [[nodiscard]] static std::chrono::nanoseconds ConvertToNanos(float time, TimeUnit unit);
        static float NanoSecToTargetUnit(TimeUnit toUnit, double nanoseconds);
        [[nodiscard]] std::chrono::nanoseconds CalculateCurrentElapsedTime(const std::chrono::nanoseconds& currentElapsed) const;

        std::chrono::nanoseconds m_TotalDuration{}; // Total duration in nanoseconds (whatever unit is given, it'll be converted to be nanoseconds firstly)
        std::chrono::steady_clock::time_point m_StartTime; // Point when timer was started (the value at here doesn't matter we will subtract this with end timer) 
        std::chrono::nanoseconds m_ElapsedTime; // Accumulated time when stopped
        bool m_IsRunning;
        bool m_IsFinished{};

        BOOST_DESCRIBE_CLASS(TimerComponent, (GameClass),
                             (m_TotalDuration, m_StartTime, m_ElapsedTime, m_IsRunning, m_IsFinished),
                             (), ())
    };
}
