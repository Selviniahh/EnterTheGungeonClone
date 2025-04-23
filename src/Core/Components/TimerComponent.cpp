#include "TimerComponent.h"

namespace ETG
{
    TimerComponent::TimerComponent(const float duration, const TimeUnit unit) : m_ElapsedTime(std::chrono::nanoseconds(0)), m_IsRunning(false)
    {
        SetDuration(duration, unit);
    }

    TimerComponent::~TimerComponent() = default;

    void TimerComponent::SetDuration(const float newDuration, const TimeUnit unit)
    {
        m_TotalDuration = ConvertToNanos(newDuration, unit);
        m_IsFinished = false;
    }

    void TimerComponent::Update()
    {
        if (m_IsRunning && !m_IsFinished)
        {
            if (CalculateCurrentElapsedTime(m_ElapsedTime) >= m_TotalDuration)
            {
                m_IsFinished = true;
                OnTimerFinished.Broadcast();
            }
        }
    }

    void TimerComponent::Start()
    {
        if (!m_IsRunning)
        {
            m_IsRunning = true;
            m_StartTime = std::chrono::steady_clock::now();
            OnTimerStarted.Broadcast();
        }
    }

    void TimerComponent::Stop()
    {
        if (m_IsRunning)
        {
            m_IsRunning = false;
            m_StartTime = std::chrono::steady_clock::now(); //Register the current start time point. The value at here doesn't matter.
        }
    }

    void TimerComponent::Reset()
    {
        m_ElapsedTime = std::chrono::nanoseconds(0);
        m_IsFinished = false;
    }

    void TimerComponent::Restart()
    {
        Reset();
        Start();
    }

    std::chrono::nanoseconds TimerComponent::CalculateCurrentElapsedTime(const std::chrono::nanoseconds& currentElapsed) const
    {
        if (!m_IsRunning) return m_ElapsedTime;

        return currentElapsed + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - m_StartTime);
    }

    float TimerComponent::GetRemainingTime(TimeUnit unit) const
    {
        std::chrono::nanoseconds currentElapsed = CalculateCurrentElapsedTime(m_ElapsedTime);

        auto remaining = m_TotalDuration - currentElapsed;
        if (remaining < std::chrono::nanoseconds(0))
            remaining = std::chrono::nanoseconds(0);

        return ConvertTimeUnit(remaining.count(), TimeUnit::Nanosecond, unit);
    }

    float TimerComponent::GetElapsedTime(const TimeUnit unit) const
    {
        const auto currentElapsed = CalculateCurrentElapsedTime(m_ElapsedTime);

        //convert nanosec to whatever out unit is
        return ConvertTimeUnit(currentElapsed.count(), TimeUnit::Nanosecond, unit);
    }

    float TimerComponent::GetProgress() const
    {
        const auto currentElapsed = CalculateCurrentElapsedTime(m_ElapsedTime);

        const auto progress = static_cast<float>(currentElapsed.count() / m_TotalDuration.count());
        return std::min(1.0f, std::max(0.0f, progress));
    }

    bool TimerComponent::IsFinished() const
    {
        return m_IsFinished;
    }

    bool TimerComponent::IsRunning() const
    {
        return m_IsRunning;
    }

    float TimerComponent::ConvertTimeUnit(const float time, const TimeUnit fromUnit, const TimeUnit toUnit)
    {
        //Convert to nanoseconds first
        const double nanoseconds = ConvertToNanos(time, fromUnit).count();

        //Then convert nanosec to target unit
        return NanoSecToTargetUnit(toUnit, nanoseconds);
    }

    float TimerComponent::NanoSecToTargetUnit(const TimeUnit toUnit, const double nanoseconds)
    {
        switch (toUnit)
        {
        case TimeUnit::Nanosecond: return static_cast<float>(nanoseconds);
        case TimeUnit::Microsecond: return static_cast<float>(nanoseconds / 1000.0);
        case TimeUnit::Millisecond: return static_cast<float>(nanoseconds / 1000000.0);
        case TimeUnit::Second: return static_cast<float>(nanoseconds / 1000000000.0);
        case TimeUnit::Minute: return static_cast<float>(nanoseconds / 60000000000.0);
        case TimeUnit::Hour: return static_cast<float>(nanoseconds / 3600000000000.0);
        case TimeUnit::Day: return static_cast<float>(nanoseconds / 86400000000000.0);
        case TimeUnit::Week: return static_cast<float>(nanoseconds / 604800000000000.0);
        case TimeUnit::Month: return static_cast<float>(nanoseconds / 2629800000000000.0);
        case TimeUnit::Year: return static_cast<float>(nanoseconds / 31557600000000000.0);
        default: return static_cast<float>(nanoseconds);
        }
    }

    std::chrono::nanoseconds TimerComponent::ConvertToNanos(const float time, const TimeUnit unit)
    {
        double nanoseconds = time;
        switch (unit)
        {
        case TimeUnit::Nanosecond: break;
        case TimeUnit::Microsecond: nanoseconds *= 1000.0;
            break;
        case TimeUnit::Millisecond: nanoseconds *= 1000000.0;
            break;
        case TimeUnit::Second: nanoseconds *= 1000000000.0;
            break;
        case TimeUnit::Minute: nanoseconds *= 60000000000.0;
            break;
        case TimeUnit::Hour: nanoseconds *= 3600000000000.0;
            break;
        case TimeUnit::Day: nanoseconds *= 86400000000000.0;
            break;
        case TimeUnit::Week: nanoseconds *= 604800000000000.0;
            break;
        case TimeUnit::Month: nanoseconds *= 2629800000000000.0;
            break;
        case TimeUnit::Year: nanoseconds *= 31557600000000000.0;
            break;
        }

        return std::chrono::nanoseconds(static_cast<long long>(nanoseconds));
    }
}
